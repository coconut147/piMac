



#include "gpio_config.h"
#include "temp_sensor.h"
#include "statemachines.h"


// Globals
PowerState CurrentPowerState = PowerOn;



 
void setup() 
{
  // Setup inputs
  pinMode(IN_POWER_BTN, INPUT_PULLUP);

  // Setup outputs
  pinMode(OUT_POWER_5V, OUTPUT);
  pinMode(OUT_STATUS_LED, OUTPUT);
  pinMode(OUT_DISP_BACKGROUND, OUTPUT);
  pinMode(OUT_SERVO, OUTPUT); 

  
  
  // Set defaultsext 
  digitalWrite(OUT_POWER_5V, LOW);
    Serial.begin(9600);
 
  analogWrite(OUT_FAN_12V,180);
    
}



int fanspeed = 0;
int statusled = 0;

void loop() 
{
  delay(100);
  Serial.println("Current Temp:");
  Serial.println(ReadTemp());

  Serial.print ("I2C SCL: ");
  Serial.println(analogRead(IN_I2C_SCL));
  Serial.print ("I2C SDA: ");
  Serial.println(analogRead(IN_I2C_SDA));
    

      
      
  switch (CurrentPowerState)
  {
    case PowerOn:
      if(digitalRead(IN_POWER_BTN) == LOW)
      {
        CurrentPowerState = PowerOff;
        Serial.println("Switch Power off");
        digitalWrite(OUT_POWER_5V, LOW);
        analogWrite(OUT_STATUS_LED,0);
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
          analogWrite(OUT_STATUS_LED,fanspeed);
          delay(10);
        }
        analogWrite(OUT_STATUS_LED,100);
        delay(2000);
      }
      
    break;
    default:
    break;
  }
}
