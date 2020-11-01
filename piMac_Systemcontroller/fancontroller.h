#ifndef PIMAC_FANCONTROLLER_H
#define PIMAC_FANCONTROLLER_H

namespace piMac
{


    typedef enum
    {
        Off,
        Auto,
        ManualLow,
        ManualMid,
        ManualHigh
    } CoolingState;


    class fancontroller
    {
    private:
        CoolingState CurrentState;
        float CurrentTemperature;
        uint8_t CurrentFanSpeed; // Fan speed in percent
        uint8_t gpio_temp;
        uint8_t gpio_fan;
        uint32_t TimeReference;

        float ReadTemperature();
        void SetFanSpeed(uint8_t FanSpeed); // Sets PWM Value of fan to FanSpeed
        uint8_t ProportionalControl(); // Does the proportional controlling based on CurrentTemperature. Returns FanSpeed in %


    public:
        fancontroller(int gpio_fan, int gpio_temp);
        ~fancontroller();

        void Activate();
        void Deactivate();
        void SetModeAuto();
        void SetModeManualHigh();

        void Operate();
    };



}

#endif