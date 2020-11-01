
#Power statemachine

```plantuml
@startuml

state NoPower : Initial state
state PowerDown : The relay is switched off \nPi has no power, Display is off \nStatus LED is off, FAN not running
state Running : RaspberryPi is running.\nRelay is on, Display is on\nThermal control with fan\nLED off
state Shutdown : Shutdown initiated\nLED blinking, FAN running
state Booting : RaspberryPi switched on. \nFan running, Display off\nLED pulsing

[*] -> NoPower

NoPower --> PowerDown

PowerDown --> Booting : PowerButton pressed

Booting --> Running : 15s Pi Power Up 
Booting --> Running : LongPowerButton pressed 

Running --> Shutdown : LongPowerButton pressed
Running --> Shutdown : 5 seconds Pi Power Down

Shutdown --> PowerDown : 5 seconds Pi Power Down
Shutdown --> PowerDown : LongPowerButton pressed

@enduml

````


