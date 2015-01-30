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
    csv_URL += "/noaacurrents/";
    csv_URL += "DownloadPredictions";
    csv_URL += "?fmt=csv"; // CSV format
    csv_URL += "&d="; // date
    csv_URL += year;

    csv_URL += '-';
    if(month < 10)
    {
      csv_URL += "0"; // formating
    }
    csv_URL += month;

    csv_URL += '-';
    if(day < 10)
    {
      csv_URL += "0"; // formating
    }
    csv_URL += day;

    csv_URL += "&id=";
    csv_URL += weather_station; //Weather station ID
    csv_URL += "&t=24hr"; // 24 HOUR TIME
    csv_URL += "&i="; //Data Interval can be 60min 30min 6min if blank returns max/min/slack

    Serial.println(csv_URL);

    // /noaacurrents/DownloadPredictions?fmt=csv&d=2015-01-23&id=FLK1302_6&t=24hr

    Year += year; // puts the year into a string for parsing
    if(client.connect(baseurl,80))
    {
      client.println(csv_URL);
      int p = 0;
      delay(10000);
      while (client.available())
      {
        char c = client.read();

        tide_data += c;
        if(c == '\n')
        {
          if(tide_data.startsWith(Year))
          {
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
    static int i;
    if(i == max_events)
    {
        i = 0;
    }
    i++;
    String time = "";
    String day = "";



    day += tide_data.charAt(8);
    day += tide_data.charAt(9);

    event_day_data[i] = day.toInt();

    parse_time(tide_data,11,i);


    if(tide_data.charAt(18) == 'e')
    {
    event_type_data[i] = 0;
    event_rate_data[i] = string_to_float(tide_data,23);
    }
    else if(tide_data.charAt(18) == 's')
    {
    event_type_data[i] = 1;
    }
    else if(tide_data.charAt(18) == 'f')
    {
    event_type_data[i] = 2;
    event_rate_data[i] = string_to_float(tide_data,25);
    }
    tide_data="";
    return 0;
}


void CFETCH_TIDE_DATA::print_event_data()
{
  if((millis() - last_print_time) > 25000)
  {
  Serial.println("Day , time(24hr) , type 0=ebb 1=slack 2=flood , minutes((hours*60) + minutes) rate");
  Serial.println("*******************************");
  for(int x = 0; x <= max_events;x++)
  {
    String  print_string = "";
    if(event_day_data[x])
    {
      Serial.print(event_day_data[x]);
      Serial.print(" , ");
      if(event_hour[x] < 10)
      {
          Serial.print('0');
      }
      Serial.print(event_hour[x]);//day,time(mil),
      Serial.print(':');
      if(event_minute[x] < 10)
      {
          Serial.print('0');
      }
      Serial.print(event_minute[x]);
      Serial.print(" , ");
      Serial.print(event_type_data[x]);//type 0=ebb 1=slack 2=flood
      Serial.print(" , ");
      Serial.print(time_in_minutes[x]);
      Serial.print(" , ");

      //print_string += " , ";
      //print_string += event_level_data[x];//current sea level
      Serial.print(event_rate_data[x]);
      Serial.print(" , ///");
      Serial.println(x);
    }
  }
  Serial.println("*******************************");

  last_print_time = millis();
  }
}



CFETCH_TIDE_DATA::~CFETCH_TIDE_DATA()
{
  //dtor
}



