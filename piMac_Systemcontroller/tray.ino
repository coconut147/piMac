
#include "tray.h"

namespace piMac
{
    tray::tray(uint8_t gpio)
    {
        this->gpio = gpio;
        CurrentState = Closed;

        // Do some HW Setup
        pinMode(this->gpio, OUTPUT); 
        CurrentPWMValue = PWMClosed;
        analogWrite(this->gpio,CurrentPWMValue);

        CloseRequested = false;
        OpenRequested = false;
        
    }
    tray::~tray()
    {
        
    }

    void tray::Toggle()
    {
        switch (CurrentState)
        {
            case Closed:
            case Closing:
                OpenRequested = true;
                break;
            case Opened:
            case Opening:
                CloseRequested = true;
            default:
                break;
        }
    }


    void tray::Close()
    {
        CloseRequested = true;
    }
    void tray::Open()
    {
        OpenRequested = true;
    }
    void tray::Operate()
    {
        uint32_t CurrentTime = millis();
        uint8_t LastPWMValue = CurrentPWMValue;
        switch (CurrentState)
        {
            case Closed:
                CloseRequested = false;
                if(OpenRequested)
                {
                    LastChange = CurrentTime;
                    CurrentState = Opening;
                }
                break;
            case Opening:

                if((CurrentPWMValue + 20) > PWMClosed)
                {
                    if((CurrentTime - LastChange) >= 50)
                    {
                        CurrentPWMValue--;
                        LastChange = CurrentTime;
                    }    
                }
                else if(CurrentPWMValue > PWMOpened)
                {
                    if((CurrentTime - LastChange) >= 4)
                    {
                        CurrentPWMValue--;
                        LastChange = CurrentTime;
                    }    
                }
                else
                {
                    CurrentState = Opened;
                }
            
                break;
            case Opened:
                OpenRequested = false;
                if(CloseRequested)
                {
                    LastChange = CurrentTime;
                    CurrentState = Closing;
                }
                break;
            case Closing:
                if((CurrentTime - LastChange) >= 7)
                {
                    if(CurrentPWMValue < PWMClosed)
                    {
                        CurrentPWMValue++;
                        LastChange = CurrentTime;
                    }
                    else
                    {
                        CurrentState = Closed;
                    }
                }
                break;
            default:
                break;
        }

        if(LastPWMValue != CurrentPWMValue)
        {
            analogWrite(this->gpio,CurrentPWMValue);
        }
    }       
}