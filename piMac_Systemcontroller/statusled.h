#ifndef STATUS_LED_H
#define STATUS_LED_H

namespace piMac
{
    typedef enum 
    {
    LedOff,
    LedBreatheIn,
    LedBreatheOut,
    LedBlinkOn,
    LedBlinkOff
    } LEDstate;


    class statusled
    {
    private:
        LEDstate CurrentState;
        uint64_t TimeReference;
        uint8_t gpio_led;
        uint8_t CurrentBrightness;
    public:
        statusled(int gpio_led);
        ~statusled();

        void SwitchState(LEDstate);
        void Operate();
    };



}

#endif