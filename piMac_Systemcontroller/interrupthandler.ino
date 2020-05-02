#include "interrupthandler.h"


namespace piMac
{

    interrupthandler_interface* interrupthandler::RegisteredClass = NULL;
    uint8_t interrupthandler::gpio = 0;


    void interrupthandler::RegisterForInterrupt(interrupthandler_interface* interface, uint8_t gpio)
    {
        interrupthandler::gpio = gpio;
        attachInterrupt(digitalPinToInterrupt(gpio), interrupthandler::ISRRisingEdge, RISING);

        RegisteredClass = interface;
        return;
    }

    void interrupthandler::ISRFallingEdge()
    {
        attachInterrupt(digitalPinToInterrupt(gpio), interrupthandler::ISRRisingEdge, RISING);
        if(RegisteredClass != NULL)
        {
            interrupthandler::RegisteredClass->InterruptServiceRoutine(interrupthandler::gpio,FallingEdge);
        }
    }
    void interrupthandler::ISRRisingEdge()
    {
        attachInterrupt(digitalPinToInterrupt(gpio), interrupthandler::ISRFallingEdge, FALLING);
        if(RegisteredClass != NULL)
        {
            interrupthandler::RegisteredClass->InterruptServiceRoutine(interrupthandler::gpio,RisingEdge);
        }
    }
}