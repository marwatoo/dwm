#!/usr/bin/env bash
# ─────────────────────────────────────────────
# check_kdeconnect.sh
# Verify if a paired Android device is reachable
# via KDE Connect on Debian 13
# ─────────────────────────────────────────────

set -euo pipefail

# ── Colour helpers ────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

# ── Dependency check ──────────────────────────
if ! command -v kdeconnect-cli &>/dev/null; then
  echo -e "${RED}[ERROR]${RESET} 'kdeconnect-cli' not found."
  echo "Install it with:  sudo apt install kdeconnect"
  exit 1
fi

# ── Fetch paired devices ──────────────────────
ALL_PAIRED=$(kdeconnect-cli --list-devices --id-name-only 2>/dev/null) || true

if [[ -z "$ALL_PAIRED" ]]; then
  echo -e "${YELLOW}[INFO]${RESET} No paired devices found."
  echo "Pair a device first via the KDE Connect app or:"
  echo "  kdeconnect-cli --pair -d <device-id>"
  exit 0
fi

# ── Build an array of ONLY reachable device IDs ──
declare -A REACHABLE_MAP
while IFS= read -r line; do
  [[ -z "$line" ]] && continue
  id=$(awk '{print $1}' <<< "$line")
  REACHABLE_MAP["$id"]=1
done < <(kdeconnect-cli --list-available --id-name-only 2>/dev/null || true)

# ── Process each paired device ────────────────
echo -e "${CYAN}${BOLD}── KDE Connect Device Status ──────────────────${RESET}"
echo ""

TOTAL=0
CONNECTED=0

while IFS= read -r line; do
  [[ -z "$line" ]] && continue
  DEVICE_ID=$(awk '{print $1}' <<< "$line")
  DEVICE_NAME=$(cut -d' ' -f2- <<< "$line")
  TOTAL=$((TOTAL + 1))

  if [[ -n "${REACHABLE_MAP[$DEVICE_ID]+_}" ]]; then
    STATUS="${GREEN}● CONNECTED${RESET}"
    CONNECTED=$((CONNECTED + 1))
    BATTERY=$(kdeconnect-cli -d "$DEVICE_ID" --battery 2>/dev/null \
              | grep -oP '\d+%' | head -1 || echo "N/A")
    echo -e "  ${BOLD}${DEVICE_NAME}${RESET}"
    echo -e "  Status  : ${STATUS}"
    echo -e "  ID      : ${DEVICE_ID}"
    echo -e "  Battery : ${BATTERY}"
  else
    STATUS="${RED}○ OFFLINE${RESET}"
    echo -e "  ${BOLD}${DEVICE_NAME}${RESET}"
    echo -e "  Status  : ${STATUS}"
    echo -e "  ID      : ${DEVICE_ID}"
  fi
  echo ""
done <<< "$ALL_PAIRED"

# ── Summary ───────────────────────────────────
echo -e "${CYAN}${BOLD}────────────────────────────────────────────────${RESET}"
echo -e "  Paired: ${TOTAL}  |  Connected: ${GREEN}${CONNECTED}${RESET}  |  Offline: ${RED}$((TOTAL - CONNECTED))${RESET}"
echo ""

# Exit 0 = at least one connected, 1 = all offline
[[ "$CONNECTED" -gt 0 ]] && exit 0 || exit 1
