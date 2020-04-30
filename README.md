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


### Power statemachine

![alternative text](http://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/coconut147/piMac/master/piMac_Systemcontroller/piMac_Systemcontroller.ino)

 ### Open points

- How to identify if pi is off? - I2C port is always pulled up
- Rendering of plantuml in readme.md fails