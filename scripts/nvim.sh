#!/bin/bash

if [ $# -eq 0 ]; then
    exec st -c com.tui.nvim -e nvim &
else
    exec st -c com.tui.nvim -e nvim "$@" &
fi
