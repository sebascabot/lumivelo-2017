#!/usr/bin/env bash
# set -x # DEBUG
set -e # Abort script on error
trap "echo '>>>'; echo '>>> ---=== FAILURE ===---'; echo '>>>'" ERR

cd anybaud

PGM="anybaud"
PORT="/dev/ttyUSB0"
SPEED="74880" # Default ESP8266 Bootloader speed

if [ ! -x "${PGM}" ]; then
    echo "Missing program '$PGM'. Going to compile it...";
    make
fi

echo "Setting '${PORT}' to '${SPEED}' baud..."
./$PGM "$PORT" "$SPEED"
