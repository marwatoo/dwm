#!/bin/bash
# dwm config browser - browse/copy keybindings, window rules, mouse
# bindings, and autostart commands parsed straight from dwm's config.h
#
# Unlike sxhkd, dwm has no runtime config file - everything is compiled in
# from config.h, so this script parses that file directly. Point CONFIGH
# at wherever you keep your dwm source.
#
# Usage: ./dwm-hotkeys.sh [/path/to/config.h]
#
# Sections (shown in this order, each sorted alphabetically within itself):
#   Keys      - static const Key keys[]
#   Rules     - static const Rule rules[]
#   Buttons   - static const Button buttons[]
#   Autostart - static const char *const autostart[]
#
# Notes / limitations:
#  - Key/Button bindings generated via a macro call (e.g. TAGKEYS(...))
#    rather than a literal { ... } entry are not expanded - they're skipped.
#  - SHCMD("...") commands are shown in full. Bindings that spawn a named
#    char* array (e.g. {.v = termcmd}) are resolved to that array's first
#    string where possible.
#
# Perf note: everything below is done with bash builtins (parameter
# expansion, [[ =~ ]], read) instead of spawning awk/sed/echo per line -
# that per-line-fork pattern is what makes naive config.h parsers slow.
# The only thing that legitimately needs a file scan is resolve_var(), and
# even that's cached so a variable like termcmd used in several bindings
# is only looked up once for the whole run.

CONFIGH="${1:-$HOME/Projects/dwm/config.h}"

if [[ ! -f "$CONFIGH" ]]; then
    notify-send "config.h not found!" "Looked for it at $CONFIGH"
    exit 1
fi

# --- Figure out what MODKEY actually is (Mod4Mask, Mod1Mask, ...) ---
modkey_raw=$(grep -oP '^#define\s+MODKEY\s+\K\S+' "$CONFIGH")
modkey_raw=${modkey_raw:-Mod4Mask}

case "$modkey_raw" in
    Mod4Mask) modkey_sym="Super" ;;
    Mod1Mask) modkey_sym="Alt" ;;
    ControlMask) modkey_sym="Ctrl" ;;
    ShiftMask) modkey_sym="Shift" ;;
    *) modkey_sym="MOD" ;;
esac

# In-place trim via nameref - no subshell/fork, unlike `x=$(trim "$x")`.
trim_var() {
    local -n __t="$1"
    __t="${__t#"${__t%%[![:space:]]*}"}"
    __t="${__t%"${__t##*[![:space:]]}"}"
}

# Modifier mask -> display name. Writes $SYM instead of echoing, so callers
# don't need `$(...)` (which forks a subshell just to grab the output).
sym_for_mask() {
    case "$1" in
        MODKEY)      SYM="$modkey_sym" ;;
        Mod4Mask)    SYM="Super" ;;
        Mod1Mask)    SYM="Alt" ;;
        Mod5Mask)    SYM="AltGr" ;;
        ControlMask) SYM="Ctrl" ;;
        ShiftMask)   SYM="Shift" ;;
        0)           SYM="" ;;
        *)           SYM="$1" ;;
    esac
}

# Build a "Mod1+Mod2+" style prefix from a pipe-separated mask expression
# like "MODKEY|ShiftMask". Writes $MODSTR (with trailing "+" if non-empty).
build_modstr() {
    local mods="$1" p
    local -a parts
    MODSTR=""
    IFS='|' read -ra parts <<< "$mods"
    for p in "${parts[@]}"; do
        trim_var p
        sym_for_mask "$p"
        [[ -n "$SYM" ]] && MODSTR+="${SYM}+"
    done
}

# Resolve a bare identifier (e.g. "termcmd") to the first quoted string in
# its array definition elsewhere in config.h (e.g. "st"). Cached, since the
# same variable is typically referenced by several bindings and re-scanning
# the file each time is the one place a fork is hard to avoid.
declare -A VAR_CACHE
resolve_var() {
    local var="$1"
    if [[ -n "${VAR_CACHE[$var]+set}" ]]; then
        printf '%s' "${VAR_CACHE[$var]}"
        return
    fi
    local marker="*${var}["
    local startline
    startline=$(grep -Fn -- "$marker" "$CONFIGH" | head -n1 | cut -d: -f1)
    local result="$var"
    if [[ -n "$startline" ]]; then
        local combined
        combined=$(awk -v s="$startline" 'NR>=s {buf = buf $0; if ($0 ~ /;/) {print buf; exit}}' "$CONFIGH")
        if [[ -n "$combined" && "$combined" =~ \"([^\"]*)\" ]]; then
            result="${BASH_REMATCH[1]}"
        fi
    fi
    VAR_CACHE[$var]="$result"
    printf '%s' "$result"
}

# Build a "function(arg)" style description from a Key/Button entry's
# function + argument fields. Writes $DESC. Shared by the Keys and Buttons
# sections since both have this exact shape.
build_desc() {
    local func="$1" arg="$2" raw argval=""
    if [[ "$arg" =~ SHCMD\(\"(.*)\"\)[[:space:]]*$ ]]; then
        argval="${BASH_REMATCH[1]}"
    elif [[ "$arg" =~ =[[:space:]]*([^\}]+) ]]; then
        raw="${BASH_REMATCH[1]}"
        trim_var raw
        raw="${raw#\"}"; raw="${raw%\"}"
        if [[ "$raw" == \&* ]]; then
            argval="${raw#&}"
        elif [[ "$raw" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]; then
            argval=$(resolve_var "$raw")
        else
            argval="$raw"
        fi
    fi

    # spawn commands can carry a full path and a pile of flags
    # (~/.config/rofi/powermenu/type-2/powermenu.sh, pactl set-sink-volume
    # @DEFAULT_SINK@ -5%, ...) - collapse to just the program name, and
    # skip the "spawn(...)" wrapper since it's the same for every entry.
    if [[ "$func" == "spawn" ]]; then
        if [[ -n "$argval" ]]; then
            argval="${argval%% *}"
            argval="${argval##*/}"
            DESC="$argval"
        else
            DESC="spawn"
        fi
        return
    fi

    DESC="$func"
    [[ -n "$argval" ]] && DESC="$func($argval)"
}

# Pull the body of a "TYPE name[] = { ... };" array out of config.h.
# Plain substring match (index), not a regex - passing a regex containing
# \[ \] through awk -v causes escape-sequence mangling on some awk builds
# (mawk, some gawk versions) and blows up with "invalid regexp: Unmatched
# [". We don't need regex power here, just "does this line declare the
# array", so index() sidesteps the whole problem.
extract_block() {
    awk -v pat="$1" '
        index($0, pat) > 0 {flag=1; next}
        flag && /^[[:space:]]*\};/ {flag=0}
        flag {print}
    ' "$CONFIGH"
}

fmt() { printf "%-9s %-50s ┃ %s\n" "$1" "$2" "$3"; }

# =====================================================================
# Keys - static const Key keys[]
# =====================================================================
keys_entries=""
while IFS= read -r line; do
    line="${line%%//*}"
    [[ "$line" != *XK_* ]] && continue
    [[ "$line" =~ ^[[:space:]]*\{ ]] || continue

    trimmed="${line#*\{}"
    trimmed="${trimmed%\}*}"
    trimmed="${trimmed%,}"

    IFS=',' read -r mods key func arg <<< "$trimmed"
    trim_var mods; trim_var key; trim_var func; trim_var arg

    key="${key#XF86}"
    key="${key#XK_}"
    [[ -z "$key" ]] && continue

    build_modstr "$mods"
    keybinding="${MODSTR}${key}"

    build_desc "$func" "$arg"

    keys_entries+=$(fmt "Keys" "$DESC" "$keybinding")
    keys_entries+=$'\n'
done <<< "$(extract_block 'keys[]')"

# =====================================================================
# Rules - static const Rule rules[]
#   { class, instance, title, tagmask, isfloating, monitor }
# =====================================================================
rules_entries=""
while IFS= read -r line; do
    [[ "$line" =~ ^[[:space:]]*\{ ]] || continue

    trimmed="${line#*\{}"
    trimmed="${trimmed%\}*}"
    trimmed="${trimmed%,}"

    IFS=',' read -r class instance title tagmask isfloating monitor <<< "$trimmed"
    trim_var class; trim_var instance; trim_var title
    trim_var tagmask; trim_var isfloating; trim_var monitor

    class="${class#\"}"; class="${class%\"}"
    instance="${instance#\"}"; instance="${instance%\"}"
    title="${title#\"}"; title="${title%\"}"

    [[ -z "$class" ]] && continue

    desc="${class:-(any class)}"
    [[ -n "$instance" && "$instance" != "NULL" ]] && desc+=" instance=$instance"
    [[ -n "$title" && "$title" != "NULL" ]] && desc+=" title=$title"

    tagdisp="all tags"
    if [[ "$tagmask" =~ ^1[[:space:]]*\<\<[[:space:]]*([0-9]+)$ ]]; then
        tagdisp="tag ${BASH_REMATCH[1]}"
    elif [[ "$tagmask" != "0" ]]; then
        tagdisp="mask $tagmask"
    fi

    info="$tagdisp"
    [[ "$isfloating" == "1" ]] && info+=", floating"
    [[ -n "$monitor" && "$monitor" != "-1" ]] && info+=", mon $monitor"

    rules_entries+=$(fmt "Rules" "$desc" "$info")
    rules_entries+=$'\n'
done <<< "$(extract_block 'rules[]')"

# =====================================================================
# Buttons - static const Button buttons[]
#   { click, mask, button, function, arg }
# =====================================================================
buttons_entries=""
while IFS= read -r line; do
    [[ "$line" =~ ^[[:space:]]*\{ ]] || continue

    trimmed="${line#*\{}"
    trimmed="${trimmed%\}*}"
    trimmed="${trimmed%,}"

    IFS=',' read -r click mask button func arg <<< "$trimmed"
    trim_var click; trim_var mask; trim_var button; trim_var func; trim_var arg

    [[ -z "$button" ]] && continue

    click="${click#Clk}"

    build_modstr "$mask"
    combo="${MODSTR}${button}"

    build_desc "$func" "$arg"

    buttons_entries+=$(fmt "Buttons" "$DESC" "${click}: ${combo}")
    buttons_entries+=$'\n'
done <<< "$(extract_block 'buttons[]')"

# =====================================================================
# Autostart - static const char *const autostart[]
#   flat list of { "prog", "arg1", "arg2", NULL, "prog2", ..., NULL }
# =====================================================================
autostart_entries=""
autoblock=$(extract_block 'autostart[]')

declare -a cmd=()
while IFS= read -r tok; do
    [[ -z "$tok" ]] && continue
    if [[ "$tok" == "NULL" ]]; then
        if ((${#cmd[@]})); then
            prog="${cmd[0]##*/}"
            args="${cmd[*]:1}"
            autostart_entries+=$(fmt "Autostart" "$prog" "${args:-(no args)}")
            autostart_entries+=$'\n'
            cmd=()
        fi
        continue
    fi
    tok="${tok#\"}"; tok="${tok%\"}"
    cmd+=("$tok")
done < <(grep -oP '"[^"]*"|NULL' <<< "$autoblock")

if [[ -z "$keys_entries$rules_entries$buttons_entries$autostart_entries" ]]; then
    notify-send "No entries found" "Check that $CONFIGH has keys[]/rules[]/buttons[]/autostart[] arrays"
    exit 1
fi

# Sort each section alphabetically by its own description, then stack the
# sections in the fixed order Keys -> Rules -> Buttons -> Autostart.
entries=""
for section_entries in "$keys_entries" "$rules_entries" "$buttons_entries" "$autostart_entries"; do
    [[ -z "$section_entries" ]] && continue
    entries+=$(printf '%s' "$section_entries" | sort -k2)
    entries+=$'\n'
done

choice=$(printf '%s\n' "$entries" | rofi -dmenu -i -p " dwm config" -theme ~/.config/rofi/launchers/type-1/style-8.rasi | awk -F'┃' '{print $2}' | xargs)

if [[ -n "$choice" ]]; then
    echo "$choice" | xclip -selection clipboard
    notify-send "Copied" "$choice"
fi
