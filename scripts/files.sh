#!/bin/bash
# Launcher for spf in Ghostty with optional arguments
if [ $# -eq 0 ]; then
    exec ghostty --class=com.tui.spf -e spf &
else
    exec ghostty --class=com.tui.spf -e spf "$@" &
fi
