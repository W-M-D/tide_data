#include "CFETCH_TIDE_DATA.h"

CFETCH_TIDE_DATA::CFETCH_TIDE_DATA(int update_delay)
{
  UPDATE_DELAY = update_delay;//ctor
}

int CFETCH_TIDE_DATA::fetch_tide_data(WiFiClient & client,String weather_station,int  year,int month,int day)
{
  if(!client.connected() && ((millis() - last_connection_time) > UPDATE_DELAY))
  {
    String csv_URL;
    char baseurl[30];
    String Year;
    String tide_data = "";
    base_url.toCharArray(baseurl,30);
    csv_URL += "GET ";

    csv_URL += "/api/datagetter";
    csv_URL += "?product=water_level";
    csv_URL += "&date=today";
    csv_URL += "&application=NOS.COOPS.TAC.WL";
    csv_URL += "&datum=MHHW";
    csv_URL += "&station=8723214";
    csv_URL += "&time_zone=lst";
    csv_URL += "&units=metric";
    csv_URL += "&interval=h";
    csv_URL += "&format=csv";

    Serial.println(csv_URL);

        //           8723214
    Year += year; // puts the year into a string for parsing
    if(client.connect(baseurl,80))
    {
      client.println(csv_URL);
      delay(10000);
      while ( client.available())
      {
        char c = client.read();
        tide_data += c;
        if(c == '\n')
        {
          if(tide_data.startsWith(Year))
          {
            last_tide_level = event_level_data;
            parse_tide_data(tide_data,year,month,day);
          }
          tide_data = "";
        }
      }
    }
    else
    {
      Serial.println("could not connect to server");
      last_connection_time = millis();
      client.flush();
      client.stop();
      return -1;
    }
    last_connection_time = millis();
    client.flush();
    client.stop();

    return 1;
  }
}

int CFETCH_TIDE_DATA::parse_tide_data(String & tide_data ,int Year,int  Month,int  Day)
{
    int tab_offset = 0 ;
    String time = "";
    String day = "";

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

    tide_data="";
    return 0;
}


void CFETCH_TIDE_DATA::print_event_data()
{
  if((millis() - last_print_time) > 25000)
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
  Serial.println(event_level_data);

  Serial.print("Max tide level: ");
  Serial.println(max_tide_level);

  Serial.print("Min tide level: ");
  Serial.println(min_tide_level);

  Serial.print("Last tide level:");
  Serial.println(last_tide_level);

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

  last_print_time = millis();
  }
}



CFETCH_TIDE_DATA::~CFETCH_TIDE_DATA()
{
  //dtor
}



