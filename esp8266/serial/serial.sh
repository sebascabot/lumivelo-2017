SPEED="115200"
PORT="/dev/ttyUSB0"
# PORT="/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0"

exec serialport-term -p "$PORT" -b "$SPEED"
