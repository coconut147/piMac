#include "gpio_config.h"
#include "temp_sensor.h"
#include "statemachines.h"
#include "statusled.h"
#include "button.h"
#include "tray.h"
#include "fancontroller.h"

using namespace piMac;

// Globals
PowerState CurrentPowerState = NoPower;

class statusled StatusLED(OUT_STATUS_LED);
class button PowerButton(IN_POWER_BTN);
class tray FrontTray(OUT_SERVO);
class fancontroller ChassisFan(OUT_FAN_12V,IN_TEMP);

uint32_t TimeReference;

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


void PiPowerOn()
{
  digitalWrite(OUT_POWER_5V, HIGH);
}
void PiPowerOff()
{
  digitalWrite(OUT_POWER_5V, LOW);
}

bool isPiOn()
{
    int rpi_tx_voltage; // stores the Raspberries Serial TX voltage
  rpi_tx_voltage = analogRead(IN_RPI_TX);
  if(rpi_tx_voltage > 500)
  {
    return true;
  }
  else
  {
    return false;
  }  
}

void DisplayPowerOn()
{
  analogWrite(OUT_DISP_BACKGROUND,255);
}
void DisplayPowerOff()
{
  analogWrite(OUT_DISP_BACKGROUND,0);
}


uint8_t BootCounter = 0;

void EnterPowerState(PowerState NewState)
{
  switch (NewState)
  {
    case PowerDown:
      Serial.println("Entering PowerDown");
      FrontTray.Close();
      StatusLED.SwitchState(LedOff);
      DisplayPowerOff();
      PiPowerOff();
      break;
    case Booting:
      Serial.println("Entering Booting");
      BootCounter = 0;
      PiPowerOn();
      ChassisFan.SetModeManualHigh();
      DisplayPowerOff();
      StatusLED.SwitchState(LedBreatheIn); 
      break;
    case Running:
      Serial.println("Entering Running");
      DisplayPowerOn();
      ChassisFan.SetModeAuto();
      StatusLED.SwitchState(LedOff);
      break;
    case Shutdown:
      Serial.println("Entering Shutdown");
      StatusLED.SwitchState(LedBlinkOn);
      break;
  }
  TimeReference = millis();
  CurrentPowerState = NewState;

}

bool wasPiOn = false;


void loop() 
{

  StatusLED.Operate();    
  FrontTray.Operate();   
  ChassisFan.Operate();   
  buttonaction LastButton = PowerButton.GetLastButtonAction();

  switch (CurrentPowerState)
  {
    case NoPower:
      EnterPowerState(PowerDown);
      break;
    case PowerDown:
      if(LastButton == ShortPress)
      {
        EnterPowerState(Booting);
      }
      break;
    case Booting:
      if(!isPiOn())
      {
        TimeReference = millis();
      }
      if (millis() - TimeReference > 15000 || BootCounter == 9)
      {
        EnterPowerState(Running);
      }
      
      if(LastButton == ShortPress)
      {
        FrontTray.Toggle();
      }
      else if (LastButton == LongPress)
      {
        EnterPowerState(Running);
      }

      break;
    case Running:
      if(LastButton == ShortPress)
      {
        FrontTray.Toggle();
      }
      else if (LastButton == LongPress)
      {
        EnterPowerState(Shutdown);
      }
      if(isPiOn())
      {
        TimeReference = millis();
      }
      if (millis() - TimeReference > 5000)
      {
        EnterPowerState(Shutdown);
      }
      break;
    case Shutdown:
      if(LastButton == ShortPress)
      {
        FrontTray.Toggle();
      }
      else if (LastButton == LongPress)
      {
        EnterPowerState(PowerDown);
      }
      if(isPiOn())
      {
        TimeReference = millis();
      }
      if (millis() - TimeReference > 5000)
      {
        EnterPowerState(PowerDown);
      }
      break;

    default:
    break;
  }

  if(isPiOn())
  {
    if(!wasPiOn)
    {
      Serial.println("Detected pi Power ON");
      wasPiOn = true;
      BootCounter++;
    }
  }
  else
  {
    if(wasPiOn)
    {
      Serial.println("Detected pi Power OFF");
      wasPiOn = false;       
    }
  }

}
