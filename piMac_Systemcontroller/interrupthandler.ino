#include "interrupthandler.h"


namespace piMac
{
    interrupthandler_interface* interrupthandler::RegisteredClass = NULL;
    uint8_t interrupthandler::gpio = 0;
    int interrupthandler::LastLevel = HIGH;

    void interrupthandler::RegisterForInterrupt(interrupthandler_interface* interface, uint8_t gpio)
    {
        interrupthandler::gpio = gpio;
        attachInterrupt(digitalPinToInterrupt(gpio), interrupthandler::ISRChange, CHANGE);

        RegisteredClass = interface;
        LastLevel = digitalRead(interrupthandler::gpio);

        return;
    }
    void interrupthandler::ISRChange()
    {
        int CurrentLevel = digitalRead(interrupthandler::gpio);        
        if(CurrentLevel != LastLevel && RegisteredClass != NULL)
        {
            if(CurrentLevel == HIGH)
            {
                interrupthandler::RegisteredClass->InterruptServiceRoutine(interrupthandler::gpio,RisingEdge);
            }
            else
            {
                interrupthandler::RegisteredClass->InterruptServiceRoutine(interrupthandler::gpio,FallingEdge);
            }
        }
        LastLevel = CurrentLevel;
    }

    
}