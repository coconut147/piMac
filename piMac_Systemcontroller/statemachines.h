/* 
* Contains all statemachine definitions
*/
#ifndef STATEMACHINES_H
#define STATEMACHINES_H


typedef enum 
{
  NoPower,
  Booting,
  PowerDown,
  Running,
  Shutdown
} PowerState;


#endif
