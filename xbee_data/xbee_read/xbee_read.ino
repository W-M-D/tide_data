
#include "Arduino.h"
#include "xb_data.h"
#include <SoftwareSerial.h>
SoftwareSerial XBee(2, 3); 
xb_data data;

void setup()
{
  Serial.begin(9600);
  XBee.begin(9600);
}

void loop()
{
  if(data.read_incomming(XBee) == 1)
  {
    Serial.print("Temperature :");
    Serial.println(data.Gettemperature());
    
    Serial.print("humidity :");
    Serial.println(data.Gethumidity());
    
    Serial.print("wind_speed :");
    Serial.println(data.Getwind_speed());
    
    Serial.print("wind_direction :");
    Serial.println(data.Getwind_direction());
    Serial.println();
    
    Serial.println("***************************************");
  }
  
}
