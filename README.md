# lumivelo-2017

## High priority task

- Create mesh network. ESP in both mode: access point and client
- Calculate distance between ESP using 'Received signal strength indication' (RSSI)
- Over the air (OTA) Upgrade
- Sleep mode (minimize current consumption while not in use)

- Choose power input module
  - Accept inverted polarity
  - wide range: 6V to 36V

- Choose ESP
  - ESP-07 (ceramic)
  - ESP-12F (cheap)
  - RobotDyn (32 M)

- Choose Firmware
  - FreeRTOS
  - NodeMCU
  - Arduino

## Risks

- OTA Upgrade with the ESP (Make it work)
- Select best fit ESP module
- Select best DC-DC step-down module
- proximity sensing: use touch module
- Clear case: selection
- Power caracterisation
  - 5V DEL consumption
  - 3.3V ESP consumption
- ESP Firmware comparison
  - RTOS
  - NONOS
  - Arduino
  - NodeMCU

  - Official AT firmware [1.3k] (115200 baud)
  - Official FreeRTOS firmware [993]
  - Electodragon custom AT firmware [686] (9600 baud with GPIO control)
  - NodeMCU [747] (Lua firmware)
  - MicroPython [612]
  - Frankenstein [500] (Das-Uboot style prompt)
  - NodeLua [765] (Lua interpreter with web IDE)


## Hardware

Surface mount is possible. Here some strategy.

- PCB manufacture, for a small fee can solder the module if we provide the BOM
- Convert a small oven into a machine to solder (ex. Piou Piou on the web)
- Find a fablab

Power module with reverse polarity protection is possible via a component build with MOSFET to minimize power lost we get with the diode.
