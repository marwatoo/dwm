#!/usr/bin/env python3
"""
apply_pertag.py — applies the pertag patch to dwm.c (marwatoo/dwm build).

Usage:
    python3 apply_pertag.py [path/to/dwm.c]

A backup is written to dwm.c.bak before any changes are made.
"""

import re
import sys
import shutil
from pathlib import Path


# ──────────────────────────────────────────────────────────────────────────────
# Core utilities
# ──────────────────────────────────────────────────────────────────────────────

def find_function(text: str, func_name: str) -> tuple[int, int]:
    """
    Find a C function by name. Returns (start_of_line, index_of_opening_brace).
    Handles all brace/whitespace styles.
    """
    pattern = re.compile(
        r'(?m)^[ \t]*(?:\w[\w\s\*]*?\s+)?' +
        re.escape(func_name) +
        r'\s*\([^)]*\)\s*\n?\s*\{',
        re.DOTALL
    )
    m = pattern.search(text)
    if not m:
        pattern2 = re.compile(
            r'(?m)^\w[^\n]*\n' +
            r'[ \t]*' + re.escape(func_name) +
            r'\s*\([^)]*\)\s*\n?\s*\{',
            re.DOTALL
        )
        m = pattern2.search(text)
    if not m:
        return -1, -1
    brace_idx = text.index('{', m.start())
    return m.start(), brace_idx


def extract_function(text: str, func_name: str) -> tuple[int, int]:
    """Return (start, end) of entire function including closing brace."""
    start, open_brace = find_function(text, func_name)
    if start == -1:
        return -1, -1
    close = matching_brace(text, open_brace)
    if close == -1:
        return -1, -1
    return start, close


def matching_brace(text: str, open_pos: int) -> int:
    """Find the closing '}' matching the '{' at open_pos."""
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


def replace_in_function(text: str, func_name: str, old_tok: str, new_tok: str) -> tuple[str, bool]:
    """Replace old_tok with new_tok, searching only inside func_name's body."""
    start, open_brace = find_function(text, func_name)
    if start == -1:
        return text, False
    close = matching_brace(text, open_brace)
    body = text[open_brace:close+1]
    new_body, ok = fuzzy_replace(body, old_tok, new_tok)
    return text[:open_brace] + new_body + text[close+1:], ok


def fuzzy_replace(text: str, old: str, new: str) -> tuple[str, bool]:
    """
    Replace `old` with `new` using token-level fuzzy matching
    (any whitespace between tokens).
    """
    tokens = re.split(r'\s+', old.strip())
    tokens = [t for t in tokens if t]
    pattern = r'\s*'.join(re.escape(t) for t in tokens)
    rx = re.compile(pattern, re.DOTALL)
    m = rx.search(text)
    if not m:
        return text, False
    return text[:m.start()] + new + text[m.end():], True


def replace_whole_function(text: str, func_name: str, new_func: str) -> tuple[str, bool]:
    """Replace an entire function definition with new_func."""
    start, close = extract_function(text, func_name)
    if start == -1:
        return text, False
    end = close + 1
    if end < len(text) and text[end] == '\n':
        end += 1
    return text[:start] + new_func + text[end:], True


def already_patched(text: str) -> bool:
    return 'Pertag' in text or 'pertag' in text


# ──────────────────────────────────────────────────────────────────────────────
# Step implementations
# ──────────────────────────────────────────────────────────────────────────────

def step1_typedef_and_monitor_field(text: str) -> tuple[str, bool]:
    """
    1a) Insert 'typedef struct Pertag Pertag;' on the line BEFORE 'struct Monitor {'.
    1b) Add 'Pertag *pertag;' after 'const Layout *lt[2];' inside Monitor struct.
    """
    # 1a: find 'struct Monitor' at the start of a line (not inside another word)
    m = re.search(r'(?m)^struct Monitor\b', text)
    if not m:
        return text, False
    insert_pos = m.start()
    text = text[:insert_pos] + 'typedef struct Pertag Pertag;\n' + text[insert_pos:]

    # 1b: find 'const Layout *lt[2];' — add Pertag *pertag; after it
    m2 = re.search(r'const Layout \*lt\[2\];', text)
    if not m2:
        return text, False
    end = m2.end()
    text = text[:end] + '\n\tPertag *pertag;' + text[end:]
    return text, True


def step2_pertag_struct(text: str) -> tuple[str, bool]:
    """Insert Pertag struct definition right after '#include "config.h"'."""
    pertag = (
        '\n'
        'struct Pertag {\n'
        '\tunsigned int curtag, prevtag; /* current and previous tag */\n'
        '\tint nmasters[LENGTH(tags) + 1]; /* number of windows in master area */\n'
        '\tfloat mfacts[LENGTH(tags) + 1]; /* mfacts per tag */\n'
        '\tunsigned int sellts[LENGTH(tags) + 1]; /* selected layouts */\n'
        '\tconst Layout *ltidxs[LENGTH(tags) + 1][2]; /* matrix of tags and layouts indexes  */\n'
        '\tint showbars[LENGTH(tags) + 1]; /* display bar for the current tag */\n'
        '};\n'
    )
    anchor = '#include "config.h"'
    idx = text.find(anchor)
    if idx == -1:
        return text, False
    end = idx + len(anchor)
    text = text[:end] + pertag + text[end:]
    return text, True


def step3_createmon(text: str) -> tuple[str, bool]:
    """Add 'unsigned int i;' local var and pertag init block in createmon()."""
    start, open_brace = find_function(text, 'createmon')
    if start == -1:
        return text, False
    close = matching_brace(text, open_brace)
    func = text[open_brace:close+1]

    # Add 'unsigned int i;' after the opening brace
    func = func[:1] + '\n\tunsigned int i;' + func[1:]

    # Add pertag init before 'return m;'
    pertag_init = (
        '\n'
        '\tm->pertag = ecalloc(1, sizeof(Pertag));\n'
        '\tm->pertag->curtag = m->pertag->prevtag = 1;\n'
        '\n'
        '\tfor (i = 0; i <= LENGTH(tags); i++) {\n'
        '\t\tm->pertag->nmasters[i] = m->nmaster;\n'
        '\t\tm->pertag->mfacts[i] = m->mfact;\n'
        '\n'
        '\t\tm->pertag->ltidxs[i][0] = m->lt[0];\n'
        '\t\tm->pertag->ltidxs[i][1] = m->lt[1];\n'
        '\t\tm->pertag->sellts[i] = m->sellt;\n'
        '\n'
        '\t\tm->pertag->showbars[i] = m->showbar;\n'
        '\t}\n'
    )
    func, ok = fuzzy_replace(func, 'return m;', pertag_init + '\treturn m;')
    if not ok:
        return text, False
    text = text[:open_brace] + func + text[close+1:]
    return text, True


def step4_incnmaster(text: str) -> tuple[str, bool]:
    old = 'selmon->nmaster = MAX(selmon->nmaster + arg->i, 0);'
    new = ('selmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag] = '
           'MAX(selmon->nmaster + arg->i, 0);')
    return replace_in_function(text, 'incnmaster', old, new)


def step5_setlayout(text: str) -> tuple[str, bool]:
    text, ok1 = replace_in_function(
        text, 'setlayout',
        'selmon->sellt ^= 1;',
        'selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag] ^= 1;'
    )
    text, ok2 = replace_in_function(
        text, 'setlayout',
        'selmon->lt[selmon->sellt] = (Layout *)arg->v;',
        ('selmon->lt[selmon->sellt] = '
         'selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt] = (Layout *)arg->v;')
    )
    return text, ok1 and ok2


def step6_setmfact(text: str) -> tuple[str, bool]:
    return replace_in_function(
        text, 'setmfact',
        'selmon->mfact = f;',
        'selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag] = f;'
    )


def step7_togglebar(text: str) -> tuple[str, bool]:
    return replace_in_function(
        text, 'togglebar',
        'selmon->showbar = !selmon->showbar;',
        ('selmon->showbar = '
         'selmon->pertag->showbars[selmon->pertag->curtag] = !selmon->showbar;')
    )


def step8_toggleview(text: str) -> tuple[str, bool]:
    new_func = (
        'void\n'
        'toggleview(const Arg *arg)\n'
        '{\n'
        '\tunsigned int newtagset = selmon->tagset[selmon->seltags] ^ (arg->ui & TAGMASK);\n'
        '\tint i;\n'
        '\n'
        '\tif (newtagset) {\n'
        '\t\tselmon->tagset[selmon->seltags] = newtagset;\n'
        '\n'
        '\t\tif (newtagset == ~0) {\n'
        '\t\t\tselmon->pertag->prevtag = selmon->pertag->curtag;\n'
        '\t\t\tselmon->pertag->curtag = 0;\n'
        '\t\t}\n'
        '\n'
        '\t\t/* test if the user did not select the same tag */\n'
        '\t\tif (!(newtagset & 1 << (selmon->pertag->curtag - 1))) {\n'
        '\t\t\tselmon->pertag->prevtag = selmon->pertag->curtag;\n'
        '\t\t\tfor (i = 0; !(newtagset & 1 << i); i++) ;\n'
        '\t\t\tselmon->pertag->curtag = i + 1;\n'
        '\t\t}\n'
        '\n'
        '\t\t/* apply settings for this view */\n'
        '\t\tselmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag];\n'
        '\t\tselmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag];\n'
        '\t\tselmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];\n'
        '\t\tselmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];\n'
        '\t\tselmon->lt[selmon->sellt^1] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt^1];\n'
        '\n'
        '\t\tif (selmon->showbar != selmon->pertag->showbars[selmon->pertag->curtag])\n'
        '\t\t\ttogglebar(NULL);\n'
        '\n'
        '\t\tfocus(NULL);\n'
        '\t\tarrange(selmon);\n'
        '\t}\n'
        '}\n'
    )
    return replace_whole_function(text, 'toggleview', new_func)


def step9_view(text: str) -> tuple[str, bool]:
    new_func = (
        'void\n'
        'view(const Arg *arg)\n'
        '{\n'
        '\tint i;\n'
        '\tunsigned int tmptag;\n'
        '\n'
        '\tif ((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])\n'
        '\t\treturn;\n'
        '\tselmon->seltags ^= 1; /* toggle sel tagset */\n'
        '\tif (arg->ui & TAGMASK) {\n'
        '\t\tselmon->tagset[selmon->seltags] = arg->ui & TAGMASK;\n'
        '\t\tselmon->pertag->prevtag = selmon->pertag->curtag;\n'
        '\n'
        '\t\tif (arg->ui == ~0)\n'
        '\t\t\tselmon->pertag->curtag = 0;\n'
        '\t\telse {\n'
        '\t\t\tfor (i = 0; !(arg->ui & 1 << i); i++) ;\n'
        '\t\t\tselmon->pertag->curtag = i + 1;\n'
        '\t\t}\n'
        '\t} else {\n'
        '\t\ttmptag = selmon->pertag->prevtag;\n'
        '\t\tselmon->pertag->prevtag = selmon->pertag->curtag;\n'
        '\t\tselmon->pertag->curtag = tmptag;\n'
        '\t}\n'
        '\n'
        '\tselmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag];\n'
        '\tselmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag];\n'
        '\tselmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];\n'
        '\tselmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];\n'
        '\tselmon->lt[selmon->sellt^1] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt^1];\n'
        '\n'
        '\tif (selmon->showbar != selmon->pertag->showbars[selmon->pertag->curtag])\n'
        '\t\ttogglebar(NULL);\n'
        '\n'
        '\tfocus(NULL);\n'
        '\tarrange(selmon);\n'
        '}\n'
    )
    return replace_whole_function(text, 'view', new_func)


# ──────────────────────────────────────────────────────────────────────────────
# Main
# ──────────────────────────────────────────────────────────────────────────────

STEPS = [
    ("Step 1 — typedef Pertag + Pertag *pertag field in Monitor", step1_typedef_and_monitor_field),
    ('Step 2 — Pertag struct definition after #include "config.h"', step2_pertag_struct),
    ("Step 3 — pertag init in createmon()", step3_createmon),
    ("Step 4 — incnmaster() update", step4_incnmaster),
    ("Step 5 — setlayout() update", step5_setlayout),
    ("Step 6 — setmfact() update", step6_setmfact),
    ("Step 7 — togglebar() update", step7_togglebar),
    ("Step 8 — toggleview() replacement", step8_toggleview),
    ("Step 9 — view() replacement", step9_view),
]


def main():
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path('dwm.c')

    if not path.exists():
        print(f"ERROR: '{path}' not found.")
        sys.exit(1)

    text = path.read_text(encoding='utf-8')

    if already_patched(text):
        print("⚠️  dwm.c already contains 'Pertag'/'pertag' — patch may already be applied.")
        print("   If you applied the broken version, restore from backup first:")
        print(f"   cp {path.with_suffix('.c.bak')} {path}")
        sys.exit(0)

    backup = path.with_suffix('.c.bak')
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
        print(f"\nRestore original with:  cp {backup} {path}")
    else:
        print("\n🎉 All 9 steps applied successfully!")
        print("Now run:  make clean install")


if __name__ == '__main__':
    main()
