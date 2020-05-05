#ifndef PIMAC_FANCONTROLLER_H
#define PIMAC_FANCONTROLLER_H

namespace piMac
{


    class fancontroller
    {
    private:
        float CurrentTemperature;
        uint8_t CurrentFanSpeed; // Fan speed in percent
        uint8_t gpio_temp;
        uint8_t gpio_fan;
        uint32_t TimeReference;

        float ReadTemperature();
        void SetFanSpeed(); // Sets PWM Value of fan to CurrentFanSpeed
        uint8_t ProportionalControl(); // Does the proportional controlling based on CurrentTemperature. Returns FanSpeed in %


    public:
        fancontroller(int gpio_fan, int gpio_temp);
        ~fancontroller();


        void Operate();
    };



}

#endif