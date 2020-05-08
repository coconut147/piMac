#include "gpio_config.h"
#include "temp_sensor.h"
#include "statemachines.h"
#include "statusled.h"
#include "button.h"
#include "tray.h"
#include "fancontroller.h"

using namespace piMac;

// Globals
PowerState CurrentPowerState = PowerOff;

class statusled StatusLED(OUT_STATUS_LED);
class button PowerButton(IN_POWER_BTN);
class tray FrontTray(OUT_SERVO);
class fancontroller ChassisFan(OUT_FAN_12V,IN_TEMP);
void setup() 
{
  // Setup inputs
  pinMode(IN_POWER_BTN, INPUT_PULLUP);

  // Setup outputs
  pinMode(OUT_POWER_5V, OUTPUT);
  pinMode(OUT_DISP_BACKGROUND, OUTPUT);
  
  // Set defaults
  digitalWrite(OUT_POWER_5V, LOW);
  Serial.begin(9600);
 
  
  StatusLED.SwitchState(LedBlinkOn);

  Serial.println("...do you smell the coffee!?");
}



int fanspeed = 0;
uint32_t TimeReference;

void loop() 
{
  int rpi_tx_voltage; // stores the Raspberries Serial TX voltage

  StatusLED.Operate();    
  FrontTray.Operate();   
  ChassisFan.Operate();   
  buttonaction LastButton = PowerButton.GetLastButtonAction();

  rpi_tx_voltage = analogRead(IN_RPI_TX);


  switch (CurrentPowerState)
  {
    case PowerOn:
      if (LastButton == LongPress || (millis() - TimeReference > 15000))  
      {
        StatusLED.SwitchState(LedOff);
        CurrentPowerState = PowerOff;
        Serial.println("Switch Power off");
        digitalWrite(OUT_POWER_5V, LOW);
        FrontTray.Close();
        analogWrite(OUT_FAN_12V,0); 
        analogWrite(OUT_DISP_BACKGROUND,0);
        ChassisFan.Deactivate();
      }
      else if(LastButton == ShortPress)
      {
        FrontTray.Toggle();
      }
      if(rpi_tx_voltage > 500)
      {
          analogWrite(OUT_DISP_BACKGROUND,255);
          TimeReference = millis();
      }
      else
      {
          analogWrite(OUT_DISP_BACKGROUND,0);
      }


    break;
    case PowerOff:
      if(LastButton == ShortPress)
      {
        StatusLED.SwitchState(LedBreatheIn);
        CurrentPowerState = PowerOn;
        Serial.println("Switch Power on");
        digitalWrite(OUT_POWER_5V, HIGH);       
 
        
        ChassisFan.Activate();
        TimeReference = millis();
      }
      
    break;
    default:
    break;
  }
  
}
