# Interrupthandler

To use interrupts inside of a C++ class on an Arduino some funny concepts are available. (e.g. making fun with ld and interruptvectors) 
To make things simple this class implements a very basic observer pattern. 

## Usage 

An instance of a class that inherits from ``ìnterrupthandler_interface`` can register by calling ``void RegisterForInterrupt(interrupthandler_interface* interface, uint8_t gpio)``.
If a GPIO Interrupt occurs this instance gets informed by a call to ``void InterruptServiceRoutine(uint8_t gpio, edge event)``

The Interrupthandler is a static class that holds some global variables & static methods.
It cannot be instanciated, the c'tor is private. 

## Open points

Please note that this concept is only used for one GPIO and only one instance is using it currently. 

If you need more than one instance registered, you need to extend this class to support more than one observer. 
