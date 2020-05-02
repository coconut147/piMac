



#include "gpio_config.h"
#include "temp_sensor.h"
#include "statemachines.h"
#include "statusled.h"
#include "button.h"

using namespace piMac;

// Globals
PowerState CurrentPowerState = PowerOff;

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


  analogWrite(OUT_SERVO,170);   
  
  // Set defaultsext 
  digitalWrite(OUT_POWER_5V, LOW);
    Serial.begin(9600);
 
  analogWrite(OUT_FAN_12V,180);
    StatusLED.SwitchState(LedBlinkOn);

  Serial.println("...do you smell the coffee!?");

}



int fanspeed = 0;
int statusled = 0;
bool trayOpen = false;

void loop() 
{


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
      if (LastButton == LongPress)
      {
        StatusLED.SwitchState(LedOff);
        CurrentPowerState = PowerOff;
        Serial.println("Switch Power off");
        digitalWrite(OUT_POWER_5V, LOW);
        if(trayOpen)
        {
          Serial.println("Closing tray");
          for(int tray = 80; tray < 170; tray ++)
          {
            analogWrite(OUT_SERVO,tray);   
            delay(5);   
          }
          trayOpen = false;
        }
        analogWrite(OUT_FAN_12V,0); 
        analogWrite(OUT_DISP_BACKGROUND,0);
        
      }
      else if(LastButton == ShortPress)
      {
        if(trayOpen)
        {
          Serial.println("Closing tray");
          for(int tray = 80; tray < 170; tray ++)
          {
            analogWrite(OUT_SERVO,tray);   
            delay(5);   
          }
          StatusLED.SwitchState(LedBreatheIn);
          trayOpen = false;
        }
        else
        {
          Serial.println("Opening tray");
          for(int tray = 170; tray > 80; tray --)
          {
            analogWrite(OUT_SERVO,tray);   
            delay(5);   
          }
          StatusLED.SwitchState(LedBlinkOn);
          trayOpen = true;
        }
      }
      
    break;
    case PowerOff:
      if(LastButton == ShortPress)
      {
        StatusLED.SwitchState(LedBreatheIn);
        CurrentPowerState = PowerOn;
        Serial.println("Switch Power on");
        digitalWrite(OUT_POWER_5V, HIGH);
        

        
        analogWrite(OUT_FAN_12V,100);          
        for(fanspeed = 10; fanspeed < 255; fanspeed +=1)
        {
          analogWrite(OUT_DISP_BACKGROUND,fanspeed);
          delay(10);
        }
      }
      
    break;
    default:
    break;
  }
}
