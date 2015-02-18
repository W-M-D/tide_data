#include "CTIDE_STATION.h"
//1.0.6
CTIDE_STATION::CTIDE_STATION(int update_delay,String weather_station)
{
  WEATHER_STATION += "station=";
  WEATHER_STATION += weather_station;
  WEATHER_STATION += "&";

  UNITS=  "units=metric&";
  DATUM =  "datum=MLLW&";
  TIME_ZONE = "time_zone=GMT&";
  FORMAT = "format=csv";
  connection_lock = false;
clear_max_min_tide_level();
}

int CTIDE_STATION::fetch_recent_predictive_tide_data(WiFiClient & client)
{
  if(!client.connected() && !connection_lock)
  {
    char baseurl[] = "tidesandcurrents.noaa.gov";

    if(client.connect(baseurl,80))
    {
          client.print("GET ");
          client.print( "/api/datagetter?");
          client.print( "product=predictions&");
          client.print( "range=1&");
          client.print( DATUM);
          client.print( WEATHER_STATION);
          client.print( TIME_ZONE);
          client.print( UNITS);
          client.print( FORMAT);
          client.println();

    }
    else
    {
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
    char baseurl[] = "tidesandcurrents.noaa.gov";

    if(client.connect(baseurl,80))
    {
     String begin_date("begin_date=");
      begin_date += date_to_string(first_year,first_month,first_day);
      begin_date += "&";

      String end_date("end_date=");
      end_date += date_to_string(first_year,first_month,first_day + 1);
      end_date += "&";

      client.print("GET ");
      client.print( "/api/datagetter?");
      client.print( "product=water_level&");
      client.print( begin_date);
      client.print(end_date);
      client.print( DATUM);
      client.print( WEATHER_STATION);
      client.print( TIME_ZONE);
      client.print( UNITS);
      client.print( FORMAT);
      client.println();
    }
    else
    {
      return -1;
    }
    connection_lock = true;
    return 1;
  }
}

int CTIDE_STATION::parse_tide_data(WiFiClient & client)
{

  int lines = 0;

  if(client.available() && connection_lock )
  {

  String tide_data = " ";
  while ( client.available())
  {

    char c = client.read();
    tide_data += c;
    if(c == '\n')
    {
      String day = " ";
      lines++;
      if(tide_data.startsWith("2"))
      {
        last_tide_level = event_level_data;

        int tab_offset = 0 ;

        tab_offset = tide_data.indexOf('-');
        tab_offset = tide_data.indexOf('-',tab_offset + 1);

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
  Serial.println("Day , time(24hr) , level");
  Serial.println("*******************************");

  Serial.print(event_day_data);

  Serial.print(" , ");
  if(event_hour < 10)
  {
      Serial.print('0');
  }
  Serial.print(event_hour);//day,time(mil),
  Serial.print(':');
  if(event_minute < 10)
  {
      Serial.print('0');
  }
  Serial.print(event_minute);

  Serial.print(" , ");
  Serial.println(event_level_data,3);

  Serial.print("Max tide level: ");
  Serial.println(max_tide_level,3);

  Serial.print("Min tide level: ");
  Serial.println(min_tide_level,3);

  Serial.print("Last tide level:");
  Serial.println(last_tide_level,3);

  Serial.print("Tide %:");
  Serial.println(tide_percent_level());

  if(tide_rising_or_falling() == 0)
  {
      Serial.println("Tide is falling!");
  }
  else if(tide_rising_or_falling() == 1)
  {
      Serial.println("Tide is rising!");
  }
  else
  {
      Serial.println("Tide unchanged since last reading!");
  }

  Serial.println("*******************************");
}

CTIDE_STATION::~CTIDE_STATION()
{
  //dtor
}



