#include "xb_data.h"

xb_data::xb_data()
{
    //ctor
    weather_start_char = '@';
    tide_start_char = '^';
    send_delay = 5;
    //t   ,h , s , d ,
    //weather ==  @,100,70,100,360,checksum,
    //tide == ^,tide_level,rising_or_falling,checksum,
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
  int weather_checksum = 0;
  int tide_checksum = 0;
  int tide_level = 0;
  int ck_a = 0;
  while(XBee.available())
  {
    char c =  XBee.read();
    if(c == '@')
    {
        for(int i = 0 ; i < 5;)
        {
            c = XBee.read();
            if(c==',')
            {
                 i++;
            }
            if(i == 1)
            {
                temperature=parse_CSV(Xbee);
            }
            if(i== 2)
            {
                humidity=parse_CSV(Xbee);
            }
            if(i==3)
            {
                wind_speed=parse_CSV(Xbee);
            }
            if(i==4)
            {
                wind_direction=parse_CSV(XBee);
            }
            if(i==5)
            {
                weather_checksum=parse_CSV(XBee);
            }
        }
    }
    else if(c == '^')
    {
        for(int i =0; i < 3;)
        {
            c = Xbee.read();
            if(c==',')
            {
                 i++;
            }
            if(i == 1)
            {
                tide_level = parse_CSV(Xbee);
                ck_a += tide_level;
            }
            if(i == 2)
            {
                tide_rising_or_falling = parse_CSV(XBee);
            }
            if(i == 3)
            {
                tide_checksum = parse_CSV(XBee);
            }
        }

    }

  }




  if(check_checksum(temperature,humidity,wind_speed,wind_direction,checksum))
  {
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
  return checksum;
}



bool xb_data::check_checksum(int temperature,int humidity,int wind_speed,int wind_direction,int checksum)
{
   if(checksum == create_checksum(temperature,humidity,wind_speed,wind_direction))
   {
     return true;
   }
   else
   {
     return false;
   }
}



xb_data::~xb_data()
{
    //dtor
}
