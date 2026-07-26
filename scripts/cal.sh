#!/bin/bash
PATH_ARG="${1:-$HOME}"
exec st -c com.tui.cal -e calcurse &
