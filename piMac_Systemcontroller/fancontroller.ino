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
        SetFanSpeed(CurrentFanSpeed);
    }

    fancontroller::~fancontroller()
    {
        this->CurrentFanSpeed = 0;
        CurrentState = Off;
    }

    void fancontroller::Activate()
    {
        TimeReference = 0;
        CurrentState = Auto;
        Operate();
    }
    void fancontroller::Deactivate()
    {
        CurrentState = Off;
        Operate();
    }

    void fancontroller::SetModeAuto()
    {
        TimeReference = 0;
        CurrentState = Auto;
        Operate();
    }
    void fancontroller::SetModeManualHigh()
    {
        CurrentState = ManualHigh;
        Operate();
    }
    

    void fancontroller::Operate()
    {
        uint32_t ActualTime = millis();
        uint32_t TimeDifference = ActualTime - TimeReference;
        uint8_t NewFanSpeed = CurrentFanSpeed;

        switch(CurrentState)
        {
            case Off:
                NewFanSpeed = 0;
                break;
            case Auto:
                if(TimeDifference >= 5000)
                {
                    CurrentTemperature = ReadTemperature();
                    // Currently we have only ProportionalControl
                    NewFanSpeed = ProportionalControl(); 
                    TimeReference = ActualTime;
                    if(NewFanSpeed < 3)
                    {
                        NewFanSpeed = 3; // Minimum 3% to avoid stalling of the fan
                    }
                }
                break;
            case ManualHigh:
                NewFanSpeed = 100;
                break;
            default:
                NewFanSpeed = 100;
                break;
        }

        if(CurrentFanSpeed != NewFanSpeed)
        {
            CurrentFanSpeed = NewFanSpeed;
            SetFanSpeed(CurrentFanSpeed);
            Serial.print("Set Current Fanspeed to ");
            Serial.print(CurrentFanSpeed);
            Serial.print ("% - Temperature ");
            Serial.print(CurrentTemperature);
            Serial.println("°C");
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

    void fancontroller::SetFanSpeed(uint8_t FanSpeed)
    {
        uint8_t PWMValue;

        if(FanSpeed >= 100)
        {
            FanSpeed = 100;
        }
        PWMValue = (uint8_t)(FanSpeed * 2.55);
        analogWrite(this->gpio_fan,PWMValue);
    }

}