# How to view serial output of ESP8266

We use a nodejs tool named `serialport` terminal.

## Installation

1. Use `nvm` to install latest nodejs LTS

   a. Follow `nvm` install instruction at [https://github.com/creationix/nvm]
   b. Install latest nodejs LTS: `nvm install --lts`
   c. Make your installation the the default: `nvm alias default v6.10.1`

2. Globaly install `serialport` and lauch the terminal script 

   a. Use npm command: `npm install -g serialport`
   b. Connect with the terminal program: serialport-term -d /dev/ttyUSB0 -b 115200
      NOTE: You can use `serialport-list` to view all port and the connected one.
