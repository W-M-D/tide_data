#include "CTIDE_STATION.h"
//1.0.6
CTIDE_STATION::CTIDE_STATION(int update_delay,String weather_station)
{
  WEATHER_STATION += F("station=");
  WEATHER_STATION += weather_station;
  WEATHER_STATION += F("&");

  UNITS=  F("units=metric&");
  DATUM =  F("datum=MLLW&");
  TIME_ZONE = F("time_zone=GMT&");
  FORMAT = F("format=csv");
  API = F("/api/datagetter?");
  BASE_URL = F("tidesandcurrents.noaa.gov");
  connection_lock = false;

clear_max_min_tide_level();
}

int CTIDE_STATION::fetch_recent_predictive_tide_data(WiFiClient & client)
{
  if(!client.connected() && !connection_lock)
  {
    char baseurl[26];
    BASE_URL.toCharArray(baseurl,26);

    if(client.connect(baseurl,80))
    {
          client.print(F("GET "));
          client.print( API);
          client.print( F("product=predictions&"));
          client.print( F("range=1&"));
          client.print( DATUM);
          client.print( WEATHER_STATION);
          client.print( TIME_ZONE);
          client.print( UNITS);
          client.print( FORMAT);
          client.println();

    }
    else
    {
      client.flush();
      client.stop();
      return -1;
    }
    connection_lock = true;
    return 1;
  }
}

int CTIDE_STATION::fetch_predictive_tide_data_day(WiFiClient & client ,int & first_day,int & first_month,int & first_year)
{
  if(!client.connected() && !connection_lock)
  {

    char baseurl[26];
    BASE_URL.toCharArray(baseurl,26);

    if(client.connect(baseurl,80))
    {

      client.print(F("GET "));
      client.print( API);
      client.print( F("product=water_level&"));
     client.print(F("date=today&"));
      client.print( DATUM);
      client.print( WEATHER_STATION);
      client.print( TIME_ZONE);
      client.print( UNITS);
      client.print( FORMAT);
      client.println();
    }
    else
    {
    client.flush();
    client.stop();
      return -1;
    }
    connection_lock = true;
    return 1;
  }
}

int CTIDE_STATION::parse_tide_data(WiFiClient & client)
{

  int lines = 0;
  char newline = '\n';

  if(client.available() && connection_lock )
  {

  String tide_data = "";
  while ( client.available())
  {

    char c = client.read();
    tide_data += c;
    if(c == newline)
    {
      String day = " ";
      lines++;
      if(tide_data.startsWith("2"))
      {
        last_tide_level = event_level_data;

        int tab_offset = 0 ;
        SetYear(tide_data.toInt());
        tab_offset = tide_data.indexOf('-');
        String month = "";
        month += tide_data.charAt(tab_offset+1);
        month += tide_data.charAt(tab_offset+2);
        SetMonth(month.toInt());

        tab_offset = tide_data.indexOf('-',tab_offset + 1);
        String day = "";
        day += tide_data.charAt(tab_offset+1);
        day += tide_data.charAt(tab_offset + 2);
        SetDay(day.toInt());

        day += tide_data.charAt(tab_offset + 1);
        day += tide_data.charAt(tab_offset + 2);
        event_day_data = day.toInt();

        tab_offset = tide_data.indexOf(' ',tab_offset + 1);
        parse_time(tide_data,tab_offset + 1 );

        tab_offset = tide_data.indexOf(',',tab_offset +1);
        event_level_data = string_to_float(tide_data,tab_offset + 1);

        if(event_level_data > max_tide_level)
        {
            max_tide_level = event_level_data;
        }
        if(event_level_data != 0.0 && event_level_data < min_tide_level)
        {
            min_tide_level = event_level_data;
        }

      }
      tide_data = "";
    }
  }
   client.flush();
   client.stop();
   connection_lock = false;
 }
  return lines;

}

void CTIDE_STATION::print_event_data()
{
 Serial.println(F("Day , time(24hr) , level"));
 Serial.println(F("*******************************"));

  Serial.print(event_day_data);

  Serial.print(F(" , "));
  if(event_hour < 10)
  {
      Serial.print(F("0"));
  }
  Serial.print(event_hour);//day,time(mil),
  Serial.print(F(":"));
  if(event_minute < 10)
  {
      Serial.print(F("0"));
  }
  Serial.print(event_minute);

  Serial.print(F(" , "));
  Serial.println(event_level_data,3);

  Serial.print(F("Max tide level: "));
  Serial.println(max_tide_level,3);

  Serial.print(F("Min tide level: "));
  Serial.println(min_tide_level,3);

  Serial.print(F("Last tide level:"));
  Serial.println(last_tide_level,3);

  Serial.print(F("Tide %:"));
  Serial.println(tide_percent_level());
     Serial.print(GetYear());
     Serial.print('-');
     Serial.print(GetMonth());
     Serial.print('-');
     Serial.println(GetDay());
  if(tide_rising_or_falling() == 0)
  {
      Serial.println(F("Tide is falling!"));
  }
  else if(tide_rising_or_falling() == 1)
  {
      Serial.println(F("Tide is rising!"));
  }
  else
  {
      Serial.println(F("Tide unchanged since last reading!"));
  }

  Serial.println(F("*******************************"));
}

CTIDE_STATION::~CTIDE_STATION()
{
  //dtor
}



