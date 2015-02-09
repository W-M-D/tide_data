
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
  int t = random(-30,120);
  int h = random(0,100);
  int s = random(0,200);
  int d = random(0,360);
  
  data.Settemperature(t);
  Serial.print("Temperature :");
  Serial.println(t);
  
  data.Sethumidity(h);
  Serial.print("humidity :");
  Serial.println(h);
  
  data.Setwind_speed(s);
  Serial.print("wind_speed :");
  Serial.println(s);
  
  data.Setwind_direction(d);
  Serial.print("wind_direction :");
  Serial.println(d);
  Serial.println();
  data.send_data(XBee);
}
