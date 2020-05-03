#ifndef TRAY_H
#define TRAY_H

namespace piMac
{
    typedef enum 
    {
        Closed,
        Opening,
        Opened,
        Closing
    } TrayState;

    const uint8_t PWMClosed = 170;
    const uint8_t PWMOpened = 80;

    class tray
    {
        private:
            TrayState CurrentState;
            uint8_t CurrentPWMValue;
            bool CloseRequested;
            bool OpenRequested;
            uint32_t LastChange; // Holds a value that shows the last time the PWM vlaue has changed
            uint8_t gpio;
        public:
            tray(uint8_t gpio);
            ~tray();
            void Operate(); // Call frequently to operate all pending operations
            void Close();
            void Open();
            void Toggle();
    };
}


#endif

