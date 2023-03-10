# Growatt Solar Inverter Modbus Data to MQTT Gateway
This sketch runs on an ESP8266 and reads data from Growatt Solar Inverter over RS485 Modbus and publishes the data over MQTT. This code can be modified for any Gorwatt inverters, it has been tested on 1 phase, 2 string inverter version such as my MIN 3000 TL-XE, MIC 1500 TL-X, MIC 600 TL-X. You find attached to this repo the documentation I used to get data out of the inverter.

![Setup](/img/setup.jpg)

This sketch publishes the following information from the holding registers that store device configuration data. More on these please refer to the modbus PDF documentation. This is sent once at startup:
Enable state, SaftyFuncEn, Inverter Max output active power percent, Inverter max output reactive power percent, Normal work PV voltage, Firmware version, Control Firmware version, Serial number, Grid voltage low and high limit protect, Grid frequency low and high limit protect

The sketch also publishes the live statistics every 4 seconds. These are stored in the input registers:
Inverter run state, Input power, PV1 and PV2 voltage current and power, Output power, Grid frequency, Energy generated today and total and these for both PV1 and PV2, 3 temperatures, Inverter output PF now, Derating mode, Fault and warning codes.

## Install
Download this repository and build and flash your ESP. As I said the code works for 1 phase 2 string inverters, if you have a 3 phase inverter, or more strings the code will still work, but you will not see all the data in the device. For BOM and PCB scroll down for the relevant sections below.
You need an Arduino IDE with ESP8266 configuration added. You need a few additional libraries (see below). And before you build open the settings.h and set your credentials. I think they are self explanatory. Compile and upload.

## Wiring
For MIN solar inverters, you need to use the SYS COM port on the underside of the unit. The special connector is supplied with the inverter.

![Port](/img/comsysport.png)

Disassemble the connector (see instructions in the user manual) and use PIN3 and PIN4 for the modbus connection. It is PIN3 for RS485A1 and PIN4 for RS485B1. The pin numbers are clearly labeled on the connector inside. RS485A1 connects to the A pin (green screw terminal) on the RS485toTTL board and RS485B1 connects to B. I used a pair of thin wires from a CAT5 network cable.

![Pins](/img/portpins.png)

## Libraries
The following libraries are used beside the "Standard" ESP8266 libraries:
- FastLED by Daniel Garcia
- ModbusMaster by Doc Walker
- ArduinoOTA
- SoftwareSerial


## Topic
the topicroot can be changed in the settings.h file (default is growatt).

topic | direction | value | function
---|----|----|--
topicroot/status | publish | | send status of the ESP8266
topicroot/data   | publish | | send power state of the growatt
topicroot/error  | publish | | send error state 
topicroot/connection |publish || send connection state of the ESP8266 uses the last will of the broker
topicroot/settings | publish || send settings from growatt
topicroot/write/getSettings | subscribe |ON | initializes the resending of the settings
topicroot/write/setEnable | subscribe | ON/OFF | enable/disable the output of the growatt
topicroot/write/setMaxOutput | subscribe | 0-100 | set the output level of the growatt in percent 
topicroot/write/setStartVoltage | subscribe || set the minimum voltage oft the MPPT tracker 
topicroot/write/setModulPower | subscribe |HEX| change the type of inverter. see chapter **ModulPower command**

## ModulPower command
Read or change the type of inverter. e.g. MIC 600TL-X to MIC 1000TL-X.

Make sure that the data sheets of the two devices match, then it is probably the same hardware.
Value| Powerstage
---|---
06 | 600W
07 | 750W
0A | 1000W
0F | 1500W
14 | 2000W
19 | 2500W
1E | 3000W

**Danger!** Can lead to the destruction of the inverter.

In order to be able to execute SetModulPower, must be add **#define useModulPower   1** in settings.h.

## PCB

![Board](/img/board.jpg)

I designed a custom PCB for this board (for a previous project), you can order if from here: https://www.pcbway.com/project/shareproject/ESP8266_Modbus_board.html

These are the components you need to build this. I purchased most of them a long time ago. I don't have the original listing, so I included a similar Aliexpress listing.
- Wemos D1 mini
- RS485 to TTL converter: https://www.aliexpress.com/item/1005001621798947.html (also works with 3.3V)

With these two components you can power the board using the micro USB on the Wemos D1 mini. My PCB includes components for mains supply:
- Hi-Link 5V power supply (https://www.aliexpress.com/item/1005001484531375.html) the power can be also delivered by the USB connection of the Growatt
- Fuseholder and 1A fuse
- Optional varistor
- 5.08mm pitch KF301-2P screw terminal block

For a different Modbus project, I have designed a slightly different board:
![Board2](/img/board2.jpg)

The difference of this board:
- Components for the mains input removed (Hi-Link power supply, fuse, varistor) and it has a +5V input from an external power course, or a footprint for a small DC-DC buck converter if powered from more 9-25V.
- Added a specific pad for a neopixel modul for visual feedback.

Rest is the same.

- 4 pin DC-DC buck converter power supply module. The module I used looks like is no longer available, but I found a similar module: https://www.aliexpress.com/item/1005003512429148.html. This is configurable for different voltages with solderpads in the back, but the default is 5V.
- WS2812 Neopixel module: https://www.aliexpress.com/item/33026835790.html. This is optional, the sketch works without the neopixel as well

To make this project work (modbus communnication and the web bits (MQTT, HTTP) you can use both of my version 1 (https://www.pcbway.com/project/shareproject/ESP8266_Modbus_board.html) and version 2 (https://www.pcbway.com/project/shareproject/Modbus_to_MQTT_board_version_2_338ed64e.html) modbus PCB modules as well. I explained the differences between them in the version 2 PCBWay project page and also in the video. Feel free to order and of the modules and the same code will work on both.

And if you want to wire them yourself, these are the connections:
```
Wemod D1 mini (or other ESP)            TTL to RS485
D1                                      DE
D2                                      RE
D5                                      RO
D6                                      DI
5V                                      VCC
G(GND)                                  GND
```

## Videos

I have a few videos covering the development of this project:

[![Prototype](https://img.youtube.com/vi/Mz1dJGthIJk/0.jpg)](https://www.youtube.com/watch?v=Mz1dJGthIJk)

[![Code almost complete](https://img.youtube.com/vi/krCdt2nv3BM/0.jpg)](https://www.youtube.com/watch?v=krCdt2nv3BM)

[![Solar Integration](https://img.youtube.com/vi/SZr8mhj-O7w/0.jpg)](https://www.youtube.com/watch?v=SZr8mhj-O7w)
