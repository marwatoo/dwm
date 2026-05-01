#!/usr/bin/env python3
"""
revert_pertag.py — reverses the pertag patch from dwm.c (marwatoo/dwm build).

Usage:
    python3 revert_pertag.py [path/to/dwm.c]

A backup is written to dwm.c.pre-revert before any changes are made.
"""

import re
import sys
import shutil
from pathlib import Path


# ──────────────────────────────────────────────────────────────────────────────
# Core utilities (same as apply script)
# ──────────────────────────────────────────────────────────────────────────────

def matching_brace(text: str, open_pos: int) -> int:
    depth = 0
    i = open_pos
    in_string = in_char = in_lc = in_bc = False
    while i < len(text):
        c = text[i]
        nc = text[i+1] if i+1 < len(text) else ''
        if in_lc:
            if c == '\n': in_lc = False
        elif in_bc:
            if c == '*' and nc == '/': in_bc = False; i += 1
        elif in_string:
            if c == '\\': i += 1
            elif c == '"': in_string = False
        elif in_char:
            if c == '\\': i += 1
            elif c == "'": in_char = False
        else:
            if c == '/' and nc == '/': in_lc = True
            elif c == '/' and nc == '*': in_bc = True
            elif c == '"': in_string = True
            elif c == "'": in_char = True
            elif c == '{': depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0:
                    return i
        i += 1
    return -1


def find_function(text: str, func_name: str) -> tuple[int, int]:
    pattern = re.compile(
        r'(?m)^[ \t]*(?:\w[\w\s\*]*?\s+)?' +
        re.escape(func_name) +
        r'\s*\([^)]*\)\s*\n?\s*\{',
        re.DOTALL
    )
    m = pattern.search(text)
    if not m:
        pattern2 = re.compile(
            r'(?m)^\w[^\n]*\n[ \t]*' + re.escape(func_name) +
            r'\s*\([^)]*\)\s*\n?\s*\{',
            re.DOTALL
        )
        m = pattern2.search(text)
    if not m:
        return -1, -1
    brace_idx = text.index('{', m.start())
    return m.start(), brace_idx


def fuzzy_replace(text: str, old: str, new: str) -> tuple[str, bool]:
    tokens = [t for t in re.split(r'\s+', old.strip()) if t]
    pattern = r'\s*'.join(re.escape(t) for t in tokens)
    rx = re.compile(pattern, re.DOTALL)
    m = rx.search(text)
    if not m:
        return text, False
    return text[:m.start()] + new + text[m.end():], True


def replace_in_function(text: str, func_name: str, old: str, new: str) -> tuple[str, bool]:
    start, open_brace = find_function(text, func_name)
    if start == -1:
        return text, False
    close = matching_brace(text, open_brace)
    body = text[open_brace:close+1]
    new_body, ok = fuzzy_replace(body, old, new)
    return text[:open_brace] + new_body + text[close+1:], ok


def replace_whole_function(text: str, func_name: str, new_func: str) -> tuple[str, bool]:
    start, open_brace = find_function(text, func_name)
    if start == -1:
        return text, False
    close = matching_brace(text, open_brace)
    end = close + 1
    if end < len(text) and text[end] == '\n':
        end += 1
    return text[:start] + new_func + text[end:], True


def not_patched(text: str) -> bool:
    return 'Pertag' not in text and 'pertag' not in text


# ──────────────────────────────────────────────────────────────────────────────
# Revert steps (each undoes one apply step)
# ──────────────────────────────────────────────────────────────────────────────

def rstep1_remove_typedef_and_field(text: str) -> tuple[str, bool]:
    """Remove 'typedef struct Pertag Pertag;\n' and '\n\tPertag *pertag;'"""
    # Remove the typedef line
    text, ok1 = fuzzy_replace(text, 'typedef struct Pertag Pertag;\n', '')
    if not ok1:
        # try without trailing newline
        text, ok1 = fuzzy_replace(text, 'typedef struct Pertag Pertag;', '')

    # Remove the Pertag *pertag; field
    text, ok2 = fuzzy_replace(text, '\n\tPertag *pertag;', '')
    if not ok2:
        text, ok2 = fuzzy_replace(text, 'Pertag *pertag;', '')

    return text, ok1 and ok2


def rstep2_remove_pertag_struct(text: str) -> tuple[str, bool]:
    """Remove the entire Pertag struct definition."""
    rx = re.compile(
        r'\nstruct Pertag \{[^}]*\};\n',
        re.DOTALL
    )
    m = rx.search(text)
    if not m:
        return text, False
    return text[:m.start()] + text[m.end():], True


def rstep3_revert_createmon(text: str) -> tuple[str, bool]:
    """Remove 'unsigned int i;' and the pertag init block from createmon()."""
    start, open_brace = find_function(text, 'createmon')
    if start == -1:
        return text, False
    close = matching_brace(text, open_brace)
    func = text[open_brace:close+1]

    # Remove 'unsigned int i;'
    func, ok1 = fuzzy_replace(func, '\n\tunsigned int i;', '')

    # Remove the pertag init block (from the ecalloc line to the closing '}\n')
    pertag_block = re.compile(
        r'\n\tm->pertag\s*=\s*ecalloc\(1,\s*sizeof\(Pertag\)\);.*?\n\t\}\n',
        re.DOTALL
    )
    m = pertag_block.search(func)
    if m:
        func = func[:m.start()] + func[m.end():]
        ok2 = True
    else:
        ok2 = False

    text = text[:open_brace] + func + text[close+1:]
    return text, ok1 and ok2


def rstep4_revert_incnmaster(text: str) -> tuple[str, bool]:
    old = ('selmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag] = '
           'MAX(selmon->nmaster + arg->i, 0);')
    new = 'selmon->nmaster = MAX(selmon->nmaster + arg->i, 0);'
    return replace_in_function(text, 'incnmaster', old, new)


def rstep5_revert_setlayout(text: str) -> tuple[str, bool]:
    text, ok1 = replace_in_function(
        text, 'setlayout',
        'selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag] ^= 1;',
        'selmon->sellt ^= 1;'
    )
    text, ok2 = replace_in_function(
        text, 'setlayout',
        ('selmon->lt[selmon->sellt] = '
         'selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt] = (Layout *)arg->v;'),
        'selmon->lt[selmon->sellt] = (Layout *)arg->v;'
    )
    return text, ok1 and ok2


def rstep6_revert_setmfact(text: str) -> tuple[str, bool]:
    return replace_in_function(
        text, 'setmfact',
        'selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag] = f;',
        'selmon->mfact = f;'
    )


def rstep7_revert_togglebar(text: str) -> tuple[str, bool]:
    return replace_in_function(
        text, 'togglebar',
        ('selmon->showbar = '
         'selmon->pertag->showbars[selmon->pertag->curtag] = !selmon->showbar;'),
        'selmon->showbar = !selmon->showbar;'
    )


def rstep8_revert_toggleview(text: str) -> tuple[str, bool]:
    new_func = (
        'void\n'
        'toggleview(const Arg *arg)\n'
        '{\n'
        '\tunsigned int newtagset = selmon->tagset[selmon->seltags] ^ (arg->ui & TAGMASK);\n'
        '\n'
        '\tif (newtagset) {\n'
        '\t\tselmon->tagset[selmon->seltags] = newtagset;\n'
        '\t\tfocus(NULL);\n'
        '\t\tarrange(selmon);\n'
        '\t}\n'
        '}\n'
    )
    return replace_whole_function(text, 'toggleview', new_func)


def rstep9_revert_view(text: str) -> tuple[str, bool]:
    new_func = (
        'void\n'
        'view(const Arg *arg)\n'
        '{\n'
        '\tif ((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])\n'
        '\t\treturn;\n'
        '\tselmon->seltags ^= 1; /* toggle sel tagset */\n'
        '\tif (arg->ui & TAGMASK)\n'
        '\t\tselmon->tagset[selmon->seltags] = arg->ui & TAGMASK;\n'
        '\tfocus(NULL);\n'
        '\tarrange(selmon);\n'
        '}\n'
    )
    return replace_whole_function(text, 'view', new_func)


# ──────────────────────────────────────────────────────────────────────────────
# Main
# ──────────────────────────────────────────────────────────────────────────────

STEPS = [
    ("Step 9 — restore view()",                   rstep9_revert_view),
    ("Step 8 — restore toggleview()",              rstep8_revert_toggleview),
    ("Step 7 — restore togglebar()",               rstep7_revert_togglebar),
    ("Step 6 — restore setmfact()",                rstep6_revert_setmfact),
    ("Step 5 — restore setlayout()",               rstep5_revert_setlayout),
    ("Step 4 — restore incnmaster()",              rstep4_revert_incnmaster),
    ("Step 3 — remove pertag init in createmon()", rstep3_revert_createmon),
    ("Step 2 — remove Pertag struct",              rstep2_remove_pertag_struct),
    ("Step 1 — remove typedef + Monitor field",    rstep1_remove_typedef_and_field),
]


def main():
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path('dwm.c')

    if not path.exists():
        print(f"ERROR: '{path}' not found.")
        sys.exit(1)

    text = path.read_text(encoding='utf-8')

    if not_patched(text):
        print("⚠️  dwm.c does not contain 'Pertag'/'pertag' — pertag patch not present.")
        print("   Nothing to revert.")
        sys.exit(0)

    backup = path.with_suffix('.c.pre-revert')
    shutil.copy(path, backup)
    print(f"✅ Backup written to {backup}\n")

    failed = []
    for label, fn in STEPS:
        text, ok = fn(text)
        status = "✅" if ok else "❌ FAILED"
        print(f"{status}  {label}")
        if not ok:
            failed.append(label)

    path.write_text(text, encoding='utf-8')
    print(f"\n📄 Written to {path}")

    if failed:
        print("\n⚠️  The following steps FAILED (manual editing required):")
        for f in failed:
            print(f"   • {f}")
        print(f"\nRestore with:  cp {backup} {path}")
    else:
        print("\n🎉 All 9 steps reverted successfully!")
        print("Now run:  make clean install")


if __name__ == '__main__':
    main()
