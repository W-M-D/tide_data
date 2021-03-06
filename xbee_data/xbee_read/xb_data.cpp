#include "xb_data.h"

xb_data::xb_data()
{
    //ctor
    start_char = '@';
    end_char = '^';
    send_delay = 5;
    //t   ,h , s , d ,
    //@,100,70,100,360,checksum,
}

int xb_data::read_incomming(SoftwareSerial &XBee)
{
  String int_string = "";
  int  start_char_offset = 0;
  int end_char_offset = 0;
  int temperature;
  int humidity;
  int wind_speed;
  int wind_direction; 
  int checksum = 0; 
  while(XBee.available())
  {
    char c =  XBee.read();
    datas_string += c;
  }
  end_char_offset = datas_string.lastIndexOf(end_char);  
  start_char_offset = datas_string.lastIndexOf(start_char,end_char_offset - 2);
  
  if(end_char_offset == -1 || start_char_offset == -1) {return -3;}
  
  if(datas_string.length() > 50) 
  {
    datas_string = "";
    return -2;
  }
  

  
  temperature = parse_CSV(datas_string, start_char_offset);
  humidity = parse_CSV(datas_string, start_char_offset);
  wind_speed = parse_CSV(datas_string, start_char_offset);
  wind_direction = parse_CSV(datas_string, start_char_offset);
  checksum = parse_CSV(datas_string, start_char_offset);

  
  if(check_checksum(temperature,humidity,wind_speed,wind_direction,checksum))
  {
    datas_string = "";
    Settemperature(temperature);
    Sethumidity(humidity);
    Setwind_speed(wind_speed);
    Setwind_direction(wind_direction);
    return 1;
  }
  else
  {
    return -1;
  }
  
}
  
int xb_data::send_data(SoftwareSerial &XBee)
{
  if(!XBee.available())
  {
    String send_string = "";
    send_string += start_char;
    send_string += ',';
    send_string += Gettemperature();
    send_string += ',';
    send_string += Gethumidity();
    send_string += ',';
    send_string += Getwind_speed();
    send_string += ',';
    send_string += Getwind_direction();
    send_string += ',';
    send_string += create_checksum(Gettemperature(),Gethumidity(),Getwind_speed(),Getwind_direction());
    send_string += ',';
    send_string += end_char;
    for(int i = 0 ; i < send_string.length();i ++)
    {
      XBee.write(send_string.charAt(i));
      delay(send_delay);
    }
  }
    
}


int xb_data::create_checksum(int temperature,int humidity,int wind_speed,int wind_direction)
{
  int checksum = 0;
  checksum += temperature;
  checksum += humidity;
  checksum += wind_speed;
  checksum += wind_direction;
  
  if(checksum == 0){return -1;}
  else{return checksum;}
}



bool xb_data::check_checksum(int temperature,int humidity,int wind_speed,int wind_direction,int checksum)
{
   if(checksum == create_checksum(temperature,humidity,wind_speed,wind_direction))
   {
     return true;
   }
   else
   {
     /*
     Serial.print("Checksum a : ");
     Serial.println(create_checksum(temperature,humidity,wind_speed,wind_direction));
     Serial.print("Checksum b : ");
     Serial.println(checksum);
     Serial.println();
    */
     return false;
   }
}

      
  
xb_data::~xb_data()
{
    //dtor
}
