#include "gpio_config.h"
#include "statusled.h"


namespace piMac
{
    statusled::statusled(int gpio)
    {
        TimeReference = millis();
        this->gpio_led = gpio;
        CurrentState = LedOff;

        pinMode(this->gpio_led, OUTPUT);
        analogWrite(this->gpio_led,0);
    }

    statusled::~statusled()
    {
    }


    void statusled::SwitchState(LEDstate NewState)
    {
        CurrentState = NewState;
        TimeReference = millis();
    }

    void statusled::Operate()
    {
        LEDstate Oldstate = CurrentState;
        uint8_t OldBrightness = CurrentBrightness;
        uint64_t ActualTime = millis() - TimeReference;

        switch (CurrentState)
        {
        case LedBlinkOn:
            if(ActualTime > 500)
            {
                CurrentBrightness = 0;
                CurrentState = LedBlinkOff;
            }
            break;
        case LedBlinkOff:
            if(ActualTime > 500)
            {
                CurrentBrightness = 80;
                CurrentState = LedBlinkOn;
            }
            break;
        case LedBreatheIn:
            if(ActualTime >= 20)
            {
                if(CurrentBrightness < 100)
                {
                    CurrentBrightness+=1;
                }
                else
                {
                    CurrentState = LedBreatheOut;
                }
                TimeReference = millis();
            }
            
            break;
        case LedBreatheOut:
            if(ActualTime >= 20)
            {
                if(CurrentBrightness > 0)
                {
                    CurrentBrightness-=1;
                }
                else
                {
                    CurrentState = LedBreatheIn;
                }
                TimeReference = millis();
            }
            
            break;

        default:
            CurrentBrightness = 0;
            break;
        }

        // reset timereference in case of statechange
        if(Oldstate != CurrentState)
        {
                TimeReference = millis();
        }
        // Write Brightness if it changed
        if(OldBrightness != CurrentBrightness)
        {
            analogWrite(gpio_led,CurrentBrightness);
        }   
        
    }
}