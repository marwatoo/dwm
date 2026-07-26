#!/bin/bash
# Launcher for micro in st with proper argument handling
if [ $# -eq 0 ]; then
    exec st -c com.tui.micro -e micro &
else
    exec st -c com.tui.micro -e micro "$@" &
fi
