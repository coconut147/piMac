#ifndef PIMAC_BUTTON_H
#define PIMAC_BUTTON_H

#include "interrupthandler.h"

namespace piMac
{

    typedef enum 
    {
      Idle,
      ButtonPressed,
      ButtonReleased
    }  buttonstate;

    typedef enum
    {
        None,
        ShortPress,
        LongPress
    } buttonaction;
    
    class button : interrupthandler_interface
    {
        const uint32_t LongPressDuration = 3000; // Duration in ms
    private:
        uint8_t gpio;
        uint32_t LastRiseTime;
        uint32_t LastFallTime;
        buttonstate CurrentState;
        buttonaction LastAction;
        void OperateStatemachine(edge event);
    public:
        button(uint8_t gpio);
        ~button();

        // Return last button action and reset
        buttonaction GetLastButtonAction();
    protected: 
        void InterruptServiceRoutine(uint8_t gpio, edge event);

    };

}

#endif