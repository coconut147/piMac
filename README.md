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

![alternative text](http://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/coconut147/piMac/master/README.md)

```plantuml

@startuml

state NoPower : Initial state
state PowerDown : The relay is switched off \nPi has no power, Display is off \nStatus LED is off, FAN not running
state Running : RaspberryPi is running.\nRelay is on, Display is on\nThermal control with fan\nLED off
state Shutdown : Shutdown initiated\nWatchdog timer running\nLED blinking, FAN running
state Booting : RaspberryPi switched on. \nFan running, Display off\nLED pulsing

[*] -> NoPower

NoPower --> PowerDown
PowerDown --> Booting : PowerButton pressed
Booting --> Running : I2C active
Booting --> Running : 15s timeout
Running --> Shutdown : PowerButton pressed
Shutdown --> PowerDown : I2C inactive
Shutdown --> PowerDown : 15s timeout

'I2C messages
Running -left-> Booting : Restart message
Running -> Shutdown : Shutdown message

'Long press of the PowerButton
Running -> PowerDown : LongPowerButton pressed
Booting -> PowerDown : LongPowerButton pressed
Shutdown -> PowerDown : LongPowerButton pressed
PowerDown -> Running : LongPowerButton pressed

@enduml


```

