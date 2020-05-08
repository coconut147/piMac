#include "fancontroller.h"




namespace piMac
{


    fancontroller::fancontroller(int gpio_fan, int gpio_temp)
    {
        this->gpio_temp = gpio_temp;
        this->gpio_fan = gpio_fan;
        this->TimeReference = 0; 
        this->CurrentFanSpeed = 0;
        CurrentState = Off;
        // Setup Hardware
        pinMode(this->gpio_fan, OUTPUT);
        SetFanSpeed();
        
    }

    fancontroller::~fancontroller()
    {
        this->CurrentFanSpeed = 0;
        CurrentState = Off;
    }

    void fancontroller::Activate()
    {
        uint32_t ActualTime = millis();
        TimeReference = ActualTime;
        this->CurrentFanSpeed = 90;
        CurrentState = Auto;
        SetFanSpeed();
    }
    void fancontroller::Deactivate()
    {
        this->CurrentFanSpeed = 0;
        CurrentState = Off;
        SetFanSpeed();
    }


    void fancontroller::Operate()
    {
        uint32_t ActualTime = millis();
        uint32_t TimeDifference = ActualTime - TimeReference;
        uint8_t NewFanSpeed = CurrentFanSpeed;
        if(TimeDifference >= 5000)
        {
            CurrentTemperature = ReadTemperature();
           // Currently we have only Proportional ProportionalControl
            NewFanSpeed = ProportionalControl(); 
 
            if(CurrentFanSpeed != NewFanSpeed)
            {
                CurrentFanSpeed = NewFanSpeed;
                SetFanSpeed();
                Serial.print("Set Current Fanspeed to ");
                Serial.print(CurrentFanSpeed);
                Serial.print ("% - Temperature ");
                Serial.print(CurrentTemperature);
                Serial.println("°C");
            }

            TimeReference = ActualTime;
        }
    }

    uint8_t fancontroller::ProportionalControl()
    {
        uint8_t retval = 0;
        int16_t FanSpeed = 0;
        FanSpeed = (CurrentTemperature - 35.0) * 5.0;

        if(FanSpeed <= 0)
        {
            retval = 0;
        }
        else if(FanSpeed >= 255)
        {
            retval = 255;
        }
        else
        {
            retval = (uint8_t) FanSpeed;
        }
        return retval;
    }

    float fancontroller::ReadTemperature()
    {
        int tempReading = analogRead(this->gpio_temp);
        double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
        tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
        float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
        return tempC;
    }

    void fancontroller::SetFanSpeed()
    {
        uint8_t PWMValue;
        uint8_t FanSpeed = CurrentFanSpeed;

        switch(CurrentState)
        {
            case Off:
                FanSpeed = 0;
                break;
            case Auto:
                if(FanSpeed < 3)
                {
                    FanSpeed = 3; // Minimum 3% to avoid stalling of the fan
                }
                else if(FanSpeed >= 100)
                {
                    FanSpeed = 100;
                }
                break;
            default:
                FanSpeed = 100;
                break;
        }


        PWMValue = (uint8_t)(FanSpeed * 2.55);
        Serial.print("Set Current FanPWM to ");
        Serial.println(PWMValue);

        analogWrite(this->gpio_fan,PWMValue);
    }

}