#include "button.h"
#include "interrupthandler.h"

namespace piMac
{

    button::button(uint8_t gpio)
    {
        this->gpio = gpio;
        this->LastRiseTime = 0;
        this->LastFallTime = 0;
        this->CurrentState = Idle;
        // Setup gpio
        pinMode(this->gpio, INPUT_PULLUP);

        // Register for Interrupt
        interrupthandler::RegisterForInterrupt(this,this->gpio);
    }

    button::~button()
    {
    }

    void button::InterruptServiceRoutine(uint8_t gpio, edge event)
    {
        if(event == FallingEdge)
        {
            LastFallTime = millis();
        }
        else if (event == RisingEdge)
        {
            LastRiseTime = millis();                
        }
        this->OperateStatemachine(event);
    }

    void button::OperateStatemachine(edge event)
    {
        // Simple approach: If there is an event without an 
        // appripriate transistion, we'll ignore it.
        // Refer to button.md statemachine
        switch (CurrentState)
        {
            case Idle:
                if(event == FallingEdge)
                {
                    CurrentState = ButtonPressed;
                }    
                break;
            case ButtonPressed:
                if(event == RisingEdge)
                {
                    CurrentState = ButtonReleased;
                    // Blow up callstack and call Operate again
                    this->OperateStatemachine(event);
                }    
                break;
            case ButtonReleased:
                uint32_t Duration; // Button press in ms
                Duration = LastRiseTime - LastFallTime;

                if(Duration < 10)
                {
                    // Ignore, it just bounced
                    // Note: Do not overwrite LastAction
                }
                else if(Duration < LongPressDuration)
                {
                    // Short press
                    LastAction = ShortPress;
                }
                else
                {
                    LastAction = LongPress;
                }

                CurrentState = Idle;
                break;
            default:
            break;
        }
    }

    buttonaction button::GetLastButtonAction()
    {
        buttonaction retval = None;
        // In general it would be sufficient to return 
        // the LastAction. As we want to return a "LongPress"
        // when the Button is still pressed, we need 
        // to do somthing more.
        // TODO: Concurrency of CurrentState with ISR...

        if(CurrentState == ButtonPressed)
        {
            uint32_t Duration; // Button press in ms
            Duration  = millis() - LastFallTime;
            if(Duration >= LongPressDuration)
            {
                retval = LongPress;
                CurrentState = Idle;
            }
            else
            {
                retval = this->LastAction;
            }
        }
        else
        {
            retval = this->LastAction;
        }

        this->LastAction = None;  // Reset Last Action
        return retval;
    }


}