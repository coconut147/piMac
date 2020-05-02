#ifndef PIMAC_INTERRUPTHANDLER_H
#define PIMAC_INTERRUPTHANDLER_H

namespace piMac
{

    typedef enum
    {
        RisingEdge,
        FallingEdge
    } edge;

    class interrupthandler_interface
    {
        friend class interrupthandler;
        protected:
        virtual void InterruptServiceRoutine(uint8_t gpio, edge event);    
    };

    class interrupthandler 
    {
        private: 
        static void ISRRisingEdge();
        static void ISRFallingEdge();
        interrupthandler::interrupthandler(); // We don't want anyone to instatiate that class

        static interrupthandler_interface* RegisteredClass;
        static uint8_t gpio;

        public: 
        static void RegisterForInterrupt(interrupthandler_interface* interface, uint8_t gpio);
    };


}

#endif