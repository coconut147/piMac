
/*
@startuml

state NoPower : Initial state
state PowerDown : The relay is switched off \nPi has no power, Display is off \nStatus LED is off, FAN not running
state Running : RaspberryPi is running.\nRelay is on, Display is on\nThermal control with fan\nLED off
state Shutdown : Shutdown initiated\nWatchdog timer running\nLED blinking, FAN running
state Booting : RaspberryPi switched on. \nFan running, Display off\nLED pulsing

[*] -> NoPower

NoPower --> PowerDown
PowerDown --> Booting : PowerButton pressed
Booting --> Running : ??? Pi Power Down detected 
Booting --> Running : 15s timeout
Running --> Shutdown : PowerButton pressed
Shutdown --> PowerDown : ??? Pi Power Down detected
Shutdown --> PowerDown : 15s timeout

'I2C messages
Running -left-> Booting : Restart message
Running -> Shutdown : Shutdown message

'Long press of the PowerButton
Running -> PowerDown : LongPowerButton pressed
Booting -> PowerDown : LongPowerButton pressed
Shutdown -> PowerDown : LongPowerButton pressed
PowerDown -> Running : LongPowerButton pressed

@enduml

*/


#define IN_TEMP 0
#define IN_POWER_BTN 2
#define IN_I2C_SDA 4
#define IN_I2C_SCL 5

#define OUT_DISP_BACKGROUND 3 
#define OUT_FAN_12V 5
#define OUT_STATUS_LED 6
#define OUT_SERVO 9
#define OUT_POWER_5V 12





// Globals
typedef enum 
{
  PowerOn,
  PowerOff
} PowerState;

PowerState CurrentPowerState = PowerOn;


float ReadTemp()
{
  int tempReading = analogRead(IN_TEMP);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius

  return tempC;
}
 
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
  digitalWrite(OUT_POWER_5V, LOW)^;
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
