



#include "gpio_config.h"
#include "temp_sensor.h"
#include "statemachines.h"
#include "statusled.h"
#include "button.h"

using namespace piMac;

// Globals
PowerState CurrentPowerState = PowerOn;

class statusled StatusLED(OUT_STATUS_LED);
class button PowerButton(IN_POWER_BTN);

 
void setup() 
{
  // Setup inputs
  pinMode(IN_POWER_BTN, INPUT_PULLUP);

  // Setup outputs
  pinMode(OUT_POWER_5V, OUTPUT);
  pinMode(OUT_DISP_BACKGROUND, OUTPUT);
  pinMode(OUT_SERVO, OUTPUT); 


  
  
  // Set defaultsext 
  digitalWrite(OUT_POWER_5V, LOW);
    Serial.begin(9600);
 
  analogWrite(OUT_FAN_12V,180);
    StatusLED.SwitchState(LedBlinkOn);

  Serial.println("...do you smell the coffee!?");

}



int fanspeed = 0;
int statusled = 0;

void loop() 
{
  //delay(100);
  //Serial.println("Current Temp:");
  //Serial.println(ReadTemp());
   

  StatusLED.Operate();    
      
  buttonaction LastButton = PowerButton.GetLastButtonAction();

  if(LastButton == ShortPress)
  {
    StatusLED.SwitchState(LedBlinkOn);
    Serial.println("Short Press");
  }
  else if (LastButton == LongPress)
  {
    StatusLED.SwitchState(LedBreatheIn);
    Serial.println("Long Press");
  }
  


  switch (CurrentPowerState)
  {
    case PowerOn:
      if(digitalRead(IN_POWER_BTN) == LOW)
      {
        StatusLED.SwitchState(LedOff);
        CurrentPowerState = PowerOff;
        Serial.println("Switch Power off");
        digitalWrite(OUT_POWER_5V, LOW);
        for(int tray = 80; tray < 170; tray ++)
        {
           analogWrite(OUT_SERVO,tray);   
           delay(5);   
        }
        analogWrite(OUT_FAN_12V,0); 
        analogWrite(OUT_DISP_BACKGROUND,0);
        
        delay(2000);
      }
      
    break;
    case PowerOff:
      if(digitalRead(IN_POWER_BTN) == LOW)
      {
        StatusLED.SwitchState(LedBreatheIn);
        CurrentPowerState = PowerOn;
        Serial.println("Switch Power on");
        digitalWrite(OUT_POWER_5V, HIGH);
        
        for(int tray = 170; tray > 80; tray --)
        {
           analogWrite(OUT_SERVO,tray);   
           delay(5);   
        }
        
        analogWrite(OUT_FAN_12V,255);          
        for(fanspeed = 10; fanspeed < 255; fanspeed +=1)
        {
          analogWrite(OUT_DISP_BACKGROUND,fanspeed);
          delay(10);
        }
        delay(2000);
      }
      
    break;
    default:
    break;
  }
}
