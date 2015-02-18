#ifndef CTIDE_STATION_H
#define CTIDE_STATION_H
#include "Arduino.h"
#include <WiFi.h>
#include<stdlib.h>

class CTIDE_STATION
{
    public:
        CTIDE_STATION(int update_delay,String weather_station);
        /*
        Connects to tidesandcurrents.noaa.gov and pulls the tide rate data in CSV format takes A wifi client, a weather station ID
        the current year,month,and day.

        Returns -1 on failure to connect.
        Returns 1 on success
        */
        int connect_tide_data(WiFiClient & client );
        int fetch_current_tide_data(WiFiClient & client,String weather_station,int  year);

        int fetch_recent_predictive_tide_data(WiFiClient & client);

        int fetch_predictive_tide_data_day(WiFiClient & client ,int & first_day,int & first_month,int & first_year);


        /*
        Parses the events, in CSV format , to extract the day,time,type,and rate. It then takes these and puts them into their
        designated arrays

        Returns how many events were parsed
        */
        int parse_tide_data(WiFiClient & client);

        int clock12_to_clock24(String time,String period); //returns 0 if falling 1 if rising and -1 if unknown

        String date_to_string(int  year, int  month,int day)
        {
             String Date;
             String Year;
             String Month;
             String Day;

              if(day < 10)
              {
                Day += "0";
              }
              if (day > 31)
              {
               month++;
              }
              Day += day;

              if(month < 10)
              {
                Month += "0";
              }
              if(month > 12)
              {
               year++;
              }
              Month += month;
              Year += year;

             Date += Year;
             Date += Month;
             Date += Day;

              return Date;
        }

        int tide_rising_or_falling()
        {
          if(last_tide_level > event_level_data)
          {
              return 0;
          }
          else if(last_tide_level < event_level_data)
          {
              return 1;
          }
          else
          {
              return -1;
          }
        }

        int tide_percent_level() // returns % of max tide
        {
           int y = map(event_level_data*1000,min_tide_level*1000,max_tide_level*1000,0,100);
           return y;
        }

        void print_event_data();

        float string_to_float(const String & parse_string,int   offset)
        {
            char float_buffer[10];
            if(!parse_string)
            {
            return 0.0;
            }
            String rate= "";
            for(int u = offset; u < parse_string.length();u++)
            {
              if(parse_string.charAt(u) == ',')
              {
                break;
              }
              rate += parse_string.charAt(u);
            }
            rate.toCharArray(float_buffer,10);
            float temp  = atof(float_buffer);
            return temp;
        }

        void parse_time(String parse_string,int offset)
        {
            String minute = "";
            String hour = "";

            hour += parse_string.charAt(offset);
            hour += parse_string.charAt(offset + 1);
            minute += parse_string.charAt(offset +3);
            minute += parse_string.charAt(offset + 4);
            event_hour = hour.toInt();
            event_minute = minute.toInt();
        }


        virtual ~CTIDE_STATION();
        String Getweather_station() { return m_weather_station; }
        void Setweather_station(String val) { m_weather_station = val; }
        String Getstation_id() { return m_station_id; }
        void Setstation_id(String val) { m_station_id = val; }

        void clear_max_min_tide_level()
        {
              max_tide_level  = -100.00;
              min_tide_level = 100.00;
        }


    protected:
    private:
        // tidesandcurrents.noaa.gov/noaacurrents/DownloadPredictions?fmt=csv&d=2015-01-26&id=FLK1301_4
        String m_weather_station ;
        String m_station_id ;
        String UNITS;
        String FORMAT ;
        String DATUM ;
        String TIME_ZONE ;
        String WEATHER_STATION ;

        double max_tide_level;
        double min_tide_level;
        double last_tide_level;
        bool connection_lock;
        uint8_t event_day_data;
        uint8_t event_minute;//time(mil)minute
        uint8_t event_hour;//time(mil) hour
        double event_level_data;// rate

};

#endif // CTIDE_STATION_H
