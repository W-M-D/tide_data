#include "CFETCH_TIDE_DATA.h"
//1.0.6
CFETCH_TIDE_DATA::CFETCH_TIDE_DATA(int update_delay)
{
  UPDATE_DELAY = update_delay;//ctor
  last_connection_time = millis();
}

int CFETCH_TIDE_DATA::fetch_tide_data(WiFiClient & client,String weather_station,int  year)
{
  if(!client.connected())
  {    
    timer = millis();


    String csv_URL;
    char baseurl[] = "tidesandcurrents.noaa.gov";
    String Year;
    String tide_data = "";


    if(client.connect(baseurl,80))
    {
      client.print( "GET ");

      client.print( "/api/datagetter");
      client.print( "?product=water_level");
      client.print( "&date=today");
      client.print( "&application=NOS.COOPS.TAC.WL");
      client.print( "&datum=MHHW");
      client.print( "&station=8723214");
      client.print( "&time_zone=gmt");
      client.print( "&units=metric");
      client.print( "&interval=h");
      client.print( "&format=csv");
      client.println();
      last_connection_time = millis();
    }
    else
    {
      Serial.println("could not connect to server");
      return -1;
    }
    Serial.print("Fetching took :");
    Serial.println((millis()-timer));

    return 1;
  }
}

int CFETCH_TIDE_DATA::parse_tide_data(WiFiClient & client)
{

  int lines = 0;
  if(client.available())
  {
  timer = millis();

  String day;
  String tide_data;
  while ( client.available())
  {
    
    char c = client.read();
    tide_data += c;
    if(c == '\n')
    {
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
        if(event_level_data < min_tide_level)
        {
            min_tide_level = event_level_data;
        }

      }
      tide_data = "";
    }
  }  
    client.flush();
    client.stop();
    Serial.print("Parsing took :");
    Serial.println((millis()-timer));
  }

  return lines;
  
}


void CFETCH_TIDE_DATA::print_event_data()
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



CFETCH_TIDE_DATA::~CFETCH_TIDE_DATA()
{
  //dtor
}



