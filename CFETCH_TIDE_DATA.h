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
        int fetch_tide_data(WiFiClient & client,String weather_station,int  year,int month,int day);


        /*
        Parses the events, in CSV format , to extract the day,time,type,and rate. It then takes these and puts them into their
        designated arrays

        Returns how many events were parsed
        */
        int parse_tide_data(int  year,int  month,int  day);

        int clock12_to_clock24(String time,String period);

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

        void parse_time(String parse_string,int offset, int i)
        {
            String minute = "";
            String hour = "";

            hour += parse_string.charAt(offset);
            hour += parse_string.charAt(offset + 1);
            minute += parse_string.charAt(offset +3);
            minute += parse_string.charAt(offset + 4);

            event_hour[i] = hour.toInt();
            event_minute[i] = minute.toInt();
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
        String Getbase_url() { return base_url; }
        void Setbase_url(String val) { base_url = val; }



    protected:
    private:
        // tidesandcurrents.noaa.gov/noaacurrents/DownloadPredictions?fmt=csv&d=2015-01-26&id=FLK1301_4
        String m_weather_station;
        String m_station_id;
        int m_current_lat;
        int m_current_lon;
        const static int max_events =16;
        int m_max_tide;
        int m_min_tide;
        String base_url = "tidesandcurrents.noaa.gov";

        String events[max_events];
        int event_day_data[max_events] = {0};
        int event_minute[max_events] = {0};//time(mil)minute
        int event_hour[max_events] = {0};//time(mil) hour
        int event_type_data[max_events] = {0};//type 0=ebb 1=slack 2=flood
        float event_rate_data[max_events];// rate
        int event_level_data[max_events] ={0};//current sea level

        unsigned long last_print_time;
        unsigned long last_connection_time;
        unsigned long UPDATE_DELAY;
};

#endif // CFETCH_TIDE_DATA_H
