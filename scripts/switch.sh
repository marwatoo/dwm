#!/bin/bash

# Services
DWM_BLOCKS="dwmblocks"
SLSTATUS="slstatus"

# Detect running instances
is_running() {
    pgrep -x "$1" > /dev/null 2>&1
}

# Kill safely
stop_service() {
    if is_running "$1"; then
        killall "$1"
        sleep 0.2
    fi
}

# Start safely
start_service() {
    case "$1" in
        "$DWM_BLOCKS")
            setsid dwmblocks >/dev/null 2>&1 &
            ;;
        "$SLSTATUS")
            setsid slstatus >/dev/null 2>&1 &
            ;;
    esac
}

# Decide what to do
if is_running "$DWM_BLOCKS"; then
    echo "[status] Switching from dwmblocks → slstatus"
    stop_service "$DWM_BLOCKS"
    start_service "$SLSTATUS"

elif is_running "$SLSTATUS"; then
    echo "[status] Switching from slstatus → dwmblocks"
    stop_service "$SLSTATUS"
    start_service "$DWM_BLOCKS"

else
    # Default if none are running
    echo "[status] Neither running — starting dwmblocks"
    start_service "$DWM_BLOCKS"
fi

