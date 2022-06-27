# Growatt Solar Inverter Modbus Data to MQTT Gateway
This sketch runs on an ESP8266 and reads data from Growatt Solar Inverter over RS485 Modbus and publishes the data over MQTT. This code can be modified for any Gorwatt inverters, it has been tested on 1 phase, 2 string inverter version such as my MIN 3000 TL-XE, MIC 1500 TL-X. You find attached to this repo the documentation I used to get data out of the inverter.

![Setup](/img/setup.jpg)

This sketch publishes the following information from the holding registers that store device configuration data. More on these please refer to the modbus PDF documentation. This is sent once at startup:
SaftyFuncEn, Inverter Max output active power percent, Inverter max output reactive power percent, Normal work PV voltage, Firmware version, Control Firmware version, Serial number, Grid voltage low and high limit protect, Grid frequency low and high limit protect

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

## PCB

![Board](/img/board.jpg)

I designed a custom PCB for this board (for a previous project), you can order if from here: https://www.pcbway.com/project/shareproject/ESP8266_Modbus_board.html

These are the components you need to build this. I purchased most of them a long time ago. I don't have the original listing, so I included a similar Aliexpress listing.
- Wemos D1 mini
- RS485 to TTL converter: https://www.aliexpress.com/item/1005001621798947.html

With these two components you can power the board using the micro USB on the Wemos D1 mini. My PCB includes components for mains supply:
- Hi-Link 5V power supply (https://www.aliexpress.com/item/1005001484531375.html)
- Fuseholder and 1A fuse
- Optional varistor
- 5.08mm pitch KF301-2P screw terminal block

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

And my final review of this project

[![Final solution](https://img.youtube.com/vi/Mz1dJGthIJk/0.jpg)](https://www.youtube.com/watch?v=Mz1dJGthIJk)

Saving data to Influx DB and generating the dashboard in Grafana

[![Reporting dashbpard](https://img.youtube.com/vi/SZr8mhj-O7w/0.jpg)](https://www.youtube.com/watch?v=SZr8mhj-O7w)
