# lumivelo-2017

## Document partagé de Pascal

https://docs.google.com/spreadsheets/d/1MMSEb6D3tJwbFp2kBUpcbrTDNyLm5-AxXXlQYRbIkyA

## Amélioration pour le module:

- Auto-detection de nombre de DEL dans la bande en mesurant la consommation
  electrique, de même pour l'ordre RGB.
- S'attache et se détache rapidement du vélo
  - Exemple aucun fil à débrancher. module d'accueil sur le vélo.
  - Exemple la pile, se retire par glissière et se charge par le même principe.
- Boitier plus compact.
- Boitier avec une surface en aluminium pour la dissipation de la chaleur.
- Alimentation par connecteur «round barrel» 
- Prise 1/8 stereo pour la communication série
- Bouton Flash plus accessible
- 4 x MOSFET. (Ex. pilote bande RGBW 12V très abordable)
- Flash ESP without the need to press a button (like FeatherHuzzan)

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
  - MicroPython
    - WiFi Example
    - ESP2812B Example
    - Deep Sleep Example

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

## Stability feature

- Put in place a watch dog
- To save battery, put in place power management (Use deep sleep)
