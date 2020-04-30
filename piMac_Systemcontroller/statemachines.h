/* 
* Contains all statemachine definitions
*/
#ifndef STATEMACHINES_H
#define STATEMACHINES_H


typedef enum 
{
  PowerOn,
  PowerOff
} PowerState;

typedef enum 
{
  LedOff,
  LedBreatheIn,
  LedBreatheOut,
  LedBlinkOn,
  LedBlinkOff
} LEDstate;








#endif
