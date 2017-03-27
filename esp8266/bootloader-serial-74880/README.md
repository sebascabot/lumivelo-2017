# Instruction to view ESP8266 bootloader output

A trick is required in order to view bootloader output, since a non-standard
74880 baud rate is use by the ESP bootloader.

## First

Open your terminal program as usual.

Example:

    serialport-term -p /dev/ttyUSB0 -b 115200

## Second

Run included `anybaud` program to set the speed at 74880 baud for /dev/ttyUSB0.

Example:

    ./anybaud /dev/ttyUSB0 74880

The script `go.sh` automate this 2nd step.

### Reference

Infomation taken from http://sensornodeinfo.rockingdlabs.com/blog/2016/01/19/baud74880/
