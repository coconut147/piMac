# piMac
A project to integrate a RaspberryPi into an old iMac G4

## Systemcontroller

The project integrates an Arduino Nano. <br>
It controls the following features of the piMac:

- Status LED
- Power Supply of pi & display 
- Display Dimming
- Power Button
- Temp Sensor
- Fan speed
- Opening and closing of front door 

Sourcecode can be found in /piMac_Systemcontroller


### Statemachines

![alternative text](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/coconut147/piMac/master/piMac_Systemcontroller/statemachines.md)

 ### Open points

- Communication between pi and Systemcontroller for display control (e..g screensaver)