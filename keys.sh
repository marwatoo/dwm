#!/usr/bin/env bash
# dwmkeys — show dwm keybindings from config.h
# Usage: dwmkeys [config.h path]
# Display: dmenu / rofi / xterm / stdout (auto-detected)

CONFIG="${1:-${XDG_CONFIG_HOME:-$HOME/.config}/dwm/config.h}"
[[ -f "$CONFIG" ]] || CONFIG="${1:-$HOME/dwm/config.h}"
[[ -f "$CONFIG" ]] || CONFIG="${1:-./config.h}"

if [[ ! -f "$CONFIG" ]]; then
    echo "config.h not found. Pass the path as argument: dwmkeys /path/to/config.h" >&2
    exit 1
fi

if ! command -v gawk &>/dev/null; then
    echo "Error: gawk is required. Install it with your package manager." >&2
    exit 1
fi

OUTPUT=$(gawk '
BEGIN {
    mod["MODKEY"]      = "Mod"
    mod["ShiftMask"]   = "Shift"
    mod["ControlMask"] = "Ctrl"
    mod["Mod1Mask"]    = "Alt"
    xk["Return"] = "Enter"
    xk["space"]  = "Space"
    xk["Tab"]    = "Tab"
}

/^static const char \*/ {
    if (match($0, /static const char \*([a-zA-Z0-9_]+)\[\][^=]*=[^"]*"([^"]+)"/, m))
        vars[m[1]] = m[2]
}

/static const Key keys\[\][^{]*\{/ { in_keys=1; next }
in_keys && /^\s*\};/              { in_keys=0; next }
!in_keys                          { next }

{
    line = $0
    gsub(/\/\/[^\n]*/, "", line)
    gsub(/^[[:space:]]+/, "", line)
    gsub(/,[[:space:]]*$/, "", line)

    if (!match(line, /^\{(.+)\}$/, outer)) next
    inner = outer[1]

    n = bsplit(inner, parts)
    if (n < 4) next

    mod_raw = parts[1]; gsub(/^[[:space:]]+|[[:space:]]+$/, "", mod_raw)
    key_raw = parts[2]; gsub(/^[[:space:]]+|[[:space:]]+$/, "", key_raw)
    fname   = parts[3]; gsub(/^[[:space:]]+|[[:space:]]+$/, "", fname)
    arg_raw = parts[4]
    for (k=5; k<=n; k++) arg_raw = arg_raw "," parts[k]
    gsub(/^[[:space:]]+|[[:space:]]+$/, "", arg_raw)

    split(mod_raw, ms, "|")
    combo = ""
    for (j in ms) {
        t = ms[j]; gsub(/^[[:space:]]+|[[:space:]]+$/, "", t)
        if (t == "0") continue
        label = (t in mod) ? mod[t] : t
        combo = (combo == "") ? label : combo "+" label
    }

    kname = key_raw; sub(/^XK_/, "", kname)
    if (kname in xk) kname = xk[kname]
    combo = (combo == "") ? kname : combo "+" kname

    action = describe(fname, arg_raw)
    printf "%-28s %s\n", combo, action
}

function bsplit(s, arr,    i, ch, depth, cur, n) {
    depth=0; cur=""; n=0
    for (i=1; i<=length(s); i++) {
        ch = substr(s,i,1)
        if (ch=="{") { depth++; cur=cur ch }
        else if (ch=="}") { depth--; cur=cur ch }
        else if (ch=="," && depth==0) { arr[++n]=cur; cur="" }
        else cur=cur ch
    }
    if (cur!="") arr[++n]=cur
    return n
}

function describe(fname, arg,    c, v, varname, val, f, tag) {
    if (fname == "spawn") {
        if (match(arg, /SHCMD\("([^"]*)"\)/, c)) return "Run: " c[1]
        if (match(arg, /\{[[:space:]]*\.v[[:space:]]*=[[:space:]]*([a-zA-Z0-9_]+)/, v)) {
            varname = v[1]
            return "Run: " (varname in vars ? vars[varname] : varname)
        }
        return "Run: " arg
    }
    if (arg ~ /^\{[[:space:]]*0[[:space:]]*\}/) {
        if (fname == "togglebar")      return "Toggle bar"
        if (fname == "view")           return "Toggle previous tag"
        if (fname == "killclient")     return "Kill focused client"
        if (fname == "togglefloating") return "Toggle floating"
        if (fname == "setlayout")      return "Cycle layout"
        return fname
    }
    if (match(arg, /\{[[:space:]]*\.i[[:space:]]*=[[:space:]]*([+-]?[0-9]+)/, v)) {
        val = v[1]+0
        if (fname == "focusstack")   return (val>0 ? "Focus next window"      : "Focus previous window")
        if (fname == "rotatestack")  return (val>0 ? "Rotate stack down"      : "Rotate stack up")
        if (fname == "incnmaster")   return (val>0 ? "Increase master count"  : "Decrease master count")
        if (fname == "shiftview")    return (val>0 ? "Shift view right"       : "Shift view left")
        if (fname == "shifttag")     return (val>0 ? "Shift tag right"        : "Shift tag left")
        if (fname == "aspectresize") return (val>0 ? "Grow width"             : "Shrink width")
        if (fname == "view")         return "View tag " int(log(val)/log(2)+1.5)
        return fname " (i=" val ")"
    }
    if (match(arg, /\{[[:space:]]*\.f[[:space:]]*=[[:space:]]*([+-]?[0-9.]+)/, v)) {
        f = v[1]+0
        if (fname == "setmfact") return (f>0 ? "Increase master factor" : "Decrease master factor")
        return fname " (f=" f ")"
    }
    if (match(arg, /\{[[:space:]]*\.ui[[:space:]]*=[[:space:]]*1[[:space:]]*<<[[:space:]]*([0-9]+)/, v)) {
        tag = v[1]+1
        if (fname == "tag")  return "Move to tag " tag
        if (fname == "view") return "View tag " tag
        return fname " tag " tag
    }
    if (fname == "setlayout" && match(arg, /&layouts\[([0-9]+)\]/, v))
        return "Set layout " v[1]
    return fname " " arg
}
' "$CONFIG" | sort -t'+' -k1,1 -k2)

if [[ -z "$OUTPUT" ]]; then
    echo "No keybindings found. Make sure gawk is installed and the file is a valid dwm config.h" >&2
    exit 1
fi

TITLE="dwm keybindings — $(basename "$CONFIG")"

if command -v rofi &>/dev/null; then
    echo "$OUTPUT" | rofi -dmenu -i -no-custom \
        -p "binding" -window-title "$TITLE" \
        -theme-str 'window {width: 700px;} listview {lines: 20;} element {font: "monospace 11";}'
    exit 0
fi

if command -v dmenu &>/dev/null; then
    echo "$OUTPUT" | dmenu -i -l 20 -fn "monospace:size=11" -p "dwm keys:"
    exit 0
fi

if command -v yad &>/dev/null; then
    echo "$OUTPUT" | awk '{print $1 "\t" substr($0, index($0,$2))}' | \
        yad --list --title="$TITLE" \
            --column="Binding" --column="Action" \
            --width=600 --height=700 --no-selection
    exit 0
fi

for term in xterm st alacritty kitty; do
    if command -v "$term" &>/dev/null; then
        tmpfile=$(mktemp /tmp/dwmkeys.XXXX)
        printf "%-28s %s\n" "BINDING" "ACTION" > "$tmpfile"
        printf '%.0s─' {1..60}                >> "$tmpfile"
        printf '\n'                            >> "$tmpfile"
        echo "$OUTPUT"                         >> "$tmpfile"
        case "$term" in
            xterm)     xterm -T "$TITLE" -geometry 80x35 -fa "Monospace" -fs 11 -e "less '$tmpfile'; rm '$tmpfile'" & ;;
            st)        st -t "$TITLE" -e sh -c "less '$tmpfile'; rm '$tmpfile'" & ;;
            alacritty) alacritty --title "$TITLE" -e sh -c "less '$tmpfile'; rm '$tmpfile'" & ;;
            kitty)     kitty --title "$TITLE" sh -c "less '$tmpfile'; rm '$tmpfile'" & ;;
        esac
        exit 0
    fi
done

echo "=== $TITLE ==="
echo "$OUTPUT"
