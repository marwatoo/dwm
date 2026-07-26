#!/bin/bash

# Toggles Gammastep nightshift effect on/off (Casablanca settings)

# Gammastep command
GAMMA_CMD="gammastep -l 33.5731:-7.5898 -t 6500:3500 -m randr"

# Check if Gammastep is running
if pgrep -x "gammastep" > /dev/null; then
    # If running, kill it
    pkill gammastep
   # notify-send "Night Shift Disabled"
else
    # If not running, start it
    eval $GAMMA_CMD
   # notify-send "Night Shift Enabled"
fi

