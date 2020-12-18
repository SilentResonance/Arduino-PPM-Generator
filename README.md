# Arduino-PPM-Generator

All code presented here is WORK IN PROGRESS!

## Prelude
* This projekt was forked from <https://github.com/kolod/Arduino-PPM-Generator>.
* Due to a bug, QTSerialbus is not working on Windows or even Ubuntu Mate 18.04 in Virtualbox 6.3.0 you can find the bug report [here](https://bugreports.qt.io/browse/QTBUG-53767). Should be fixed with Qt 5.13.0 RC1. Works with my local Qt 5.13.0 on Windows. Let me know, if it works in Linux too ;)
 * Using a test message (0x01 0x03 0x00 0x00 0x00 0x01 0x8a 0x0a) with the QtSerialport blockingmaster example, created a response using the Arduino example "hello_modbusino" from the [Simple-Modbus-Slave](https://github.com/kolod/Arduino-Simple-Modbus-Slave/) library on Arduino Uno R3 SMD. Note: make shure both tools use 115200 Baud speed setting - this is not default for the Qt example...
##Project goals
 * Test GUI to create PXX, S.Bus and/or LANC output to control camera equipment like Blackmagic Micro Cinema Camera (S.Bus + LANC) or Video Assist 4k (LANC).
 * Controlling FrSky XJT module with PXX to create S.Bus output with FrSky XM Plus receiver for controlling Blackmagic Micro Cinema Camera (BMMCC).
 * Wireless LANC still no final idea (maybe S.Bus Ch 17 and 18 as Auto Exposure trigger which only seems to be accessable via LANC in BMMCC)

## Prerequisites
* Arduino IDE (> 1.8.5)
* Qt 5.13.0 & Charts module
* Simple-Modbus-Slave [library](https://github.com/kolod/Arduino-Simple-Modbus-Slave/)

## Protocols
### S.Bus
* Code from [Pawel S.](https://quadmeup.com/generate-s-bus-with-arduino-in-a-simple-way/) is used.
* An inverter is needed between Arduino TX pin to S.Bus input

### PXX
* Code from [PPMtoPXXArduino project](https://github.com/MichaelCWarren/PPMtoPXXArduino) is used

## Branches
* **master**
    * Original branch from https://github.com/kolod/Arduino-PPM-Generator
* **sbus_test** 
    * <span style="color:green">Working</span> with direct Arduino Mega &rarr; inverter &rarr; BMMCC extension port S.Bus (all at 5V)
    * Adapted GUI with limited (non-percent) value range according to [Blackmagic Micro Studio Camera Manual](https://documents.blackmagicdesign.com/UserManuals/BlackmagicStudioCameraManual.pdf) chapter "Remote Settings" page 26.
    * :black_square_button: Needs GUI/code cleanup
    
* **pxx_test** 
    * <span style="color:red">UNTESTED</span>
    * Adapted GUI with raw output values and PXX output
    
* **camera_sbus** (not yet created)
    * Based on **sbus_test** branch
    * :black_square_button: Add camera controls
    * :black_square_button: Create XML lens parameter database in order to map raw values to aperture/zoom ranges. Sadly lensfun doesn't seem to have complete aperture lists for every lens. Any other lens library out there worth looking into?

## TODOs
* ~~:ballot_box_with_check: Test serial communication on Windows with QtSerialport blockingmaster~~
* ~~:black_square_button: Rework Qt GUI to use QtSerialport instead of QtSerialbus~~
* :black_square_button: Add customizable controls to match
* :black_square_button: Extend Arduino sketch to be able to output PXX, S.Bus or LANC

## Installation Linux
1. Used Qt online installer for 64-bit
2. Install some packages necessary for builing (Ubuntu Mate 18.04)
 <pre>sudo apt-get install build-essential clang libglu1-mesa-dev</pre>

## Tested environments
### Software
* Qt 5.13.0 on Windows 10 (not working, see Prelude)
 *  Arduino IDE 1.8.9
* Qt 5.13.0 on Ubuntu Mate 18.04 on Virtualbox (not working, see Prelude)

### Hardware
* Arduino Uno R3 SMD Edition
* S.Bus might require second hardware serial, so Arduino Mega 2560 was used here
* FrSky XJT sender module
* FrSky XM Plus receiver module

## Appendix (Text from original repository)
Sketch for Arduino Nano allows you to generate a PPM signal.
It uses hardware sixteen bit timer.
The pulse duration is adjusted with an accuracy of 0.0625 microseconds.

![gui screenshot](https://raw.githubusercontent.com/SilentResonance/Arduino-PPM-Generator/master/gui.png)

Sorry, but comments in the code in Russian.

## Test Markdown Code
[ ] Testbox

<span style="text-decoration:overline">SIGNAL_ACTIVE_LOW</span>

<span style="text-decoration:line-through">SIGNAL_ACTIVE_LOW</span>