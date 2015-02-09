
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
  data.Settemperature(random(-30,120));
  data.Sethumidity(random(0,100));
  data.Setwind_speed(random(0,200));
  data.Setwind_direction(random(0,360));
  data.send_data(XBee);
}
