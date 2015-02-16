#ifndef CFETCH_TIDE_DATA_H
#define CFETCH_TIDE_DATA_H
#include "Arduino.h"
#include <WiFi.h>
#include<stdlib.h>

class CFETCH_TIDE_DATA
{
    public:
        CFETCH_TIDE_DATA(int update_delay);
        /*
        Connects to tidesandcurrents.noaa.gov and pulls the tide rate data in CSV format takes A wifi client, a weather station ID
        the current year,month,and day.

        Returns -1 on failure to connect.
        Returns 1 on success
        */
        int fetch_tide_data(WiFiClient & client,String weather_station,int  year);


        /*
        Parses the events, in CSV format , to extract the day,time,type,and rate. It then takes these and puts them into their
        designated arrays

        Returns how many events were parsed
        */
        int parse_tide_data(WiFiClient & client);

        int clock12_to_clock24(String time,String period); //returns 0 if falling 1 if rising and -1 if unknown

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
           int y = map(event_level_data*100,min_tide_level*100,max_tide_level*100,0,100);
           return y;
        }

        void print_event_data();

        float string_to_float(String parse_string,int  offset)
        {
            char float_buffer[10];
            String rate= "";
            for(int u = offset; u <= parse_string.length();u++)
            {
              rate += parse_string.charAt(u);
            }
            rate.toCharArray(float_buffer,10);
            return(atof(float_buffer));
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


        virtual ~CFETCH_TIDE_DATA();
        String Getweather_station() { return m_weather_station; }
        void Setweather_station(String val) { m_weather_station = val; }
        String Getstation_id() { return m_station_id; }
        void Setstation_id(String val) { m_station_id = val; }
        int Getcurrent_lat() { return m_current_lat; }
        void Setcurrent_lat(int val) { m_current_lat = val; }
        int Getcurrent_lon() { return m_current_lon; }
        void Setcurrent_lon(int val) { m_current_lon = val; }




    protected:
    private:
        // tidesandcurrents.noaa.gov/noaacurrents/DownloadPredictions?fmt=csv&d=2015-01-26&id=FLK1301_4
        String m_weather_station;
        String m_station_id;
        int m_current_lat;
        int m_current_lon;
        double max_tide_level;
        double min_tide_level;
        double last_tide_level;
        String base_url;

        uint8_t event_day_data;
        uint8_t event_minute;//time(mil)minute
        uint8_t event_hour;//time(mil) hour
        double event_level_data;// rate

        unsigned long last_print_time;
        unsigned long last_connection_time;
        unsigned long UPDATE_DELAY;
};

#endif // CFETCH_TIDE_DATA_H
