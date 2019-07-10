# Arduino-PPM-Generator

## Prelude
* This projekt was forked from <https://github.com/kolod/Arduino-PPM-Generator>.
* Due to a bug, QTSerialbus is not working on Windows or even Ubuntu Mate 18.04 in Virtualbox 6.3.0 you can find the bug report [here](https://bugreports.qt.io/browse/QTBUG-53767).
 * Using a test message (0x01 0x03 0x00 0x00 0x00 0x01 0x8a 0x0a) with the QtSerialport blockingmaster example, created a response using the Arduino example "hello_modbusino" from the [Simple-Modbus-Slave](https://github.com/kolod/Arduino-Simple-Modbus-Slave/) library on Arduino Uno R3 SMD. Note: make shure both tools use 115200 Baud speed setting - this is not default for the Qt example...
##Project goals
 * Test GUI to create PPM, S.Bus and/or LANC output to control camera equipment link Blackmagic Micro Cinema Camera (S.Bus) or Video Assist 4k (LANC).
 * Controling FrSky XJT module with PPM to create S.Bus Output with FrSky XM Plus receiver for controlling Blackmagic Micro Cinema Camera (BMMCC).

## Prerequisites
* Arduino IDE (> 1.8.5)
* Qt 5.13.0 & Charts module
* Simple-Modbus-Slave [library](https://github.com/kolod/Arduino-Simple-Modbus-Slave/)

## TODOs
* :ballot_box_with_check: Test serial communication on Windows with QtSerialport blockingmaster
* :black_square_button: Rework Qt GUI to use QtSerialport instead of QtSerialbus
* :black_square_button: Add customizable controls to match
* :black_square_button: Extend Arduino sketch to be able to output PPM, S.Bus or LANC

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
* FrSky XJT sender module
* FrSky XM Plus receiver module

## Appendix (Text from original repository)
Sketch for Arduino Nano allows you to generate a PPM signal.
It uses hardware sixteen bit timer.
The pulse duration is adjusted with an accuracy of 0.0625 microseconds.

![gui screenshot](https://raw.githubusercontent.com/SilentResonance/Arduino-PPM-Generator/master/gui.png)

Sorry, but comments in the code in Russian.

[ ] Testbox