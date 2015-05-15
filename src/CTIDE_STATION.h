#ifndef CTIDE_STATION_H
#define CTIDE_STATION_H
#include<stdlib.h>
#include <sstream>
#include <iostream>
#include <curl/curl.h>
#include <regex>

class CTIDE_STATION
{
public:
    CTIDE_STATION(int update_delay,std::string weather_station);
    /*
    Connects to tidesandcurrents.noaa.gov and pulls the tide rate data in CSV format takes A wifi client, a weather station ID
    the current year,month,and day.

    Returns -1 on failure to connect.
    Returns 1 on success
    */
    int connect_tide_data( );
    int fetch_current_tide_data(std::string weather_station);

    int fetch_recent_predictive_tide_data();

    int fetch_predictive_tide_data_day();

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    /*
    Parses the events, in CSV format , to extract the day,time,type,and rate. It then takes these and puts them into their
    designated arrays

    Returns how many events were parsed
    */

    void parse_time(std::string parse_string,std::string::size_type & offset)
    {
        std::string hour =" ";
        hour += parse_string.at(offset );
        hour += parse_string.at(offset + 1);

        std::string mins = " ";
        mins += parse_string.at(offset+3);
        mins += parse_string.at(offset+4);

        event_hour = std::stoi(hour);
        event_minute = std::stoi(mins);
    }
    int parse_tide_data(std::string);
    bool send_string(const std::string & tide_URL);
    /*
        Map function taken from Arduino source
    */
    long map(long x, long in_min, long in_max, long out_min, long out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    std::string date_to_string(int  year, int  month,int day)
    {
        std::string Date;
        std::string Year;
        std::string Month;
        std::string Day;

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




    virtual ~CTIDE_STATION();
    std::string Getweather_station()
    {
        return m_weather_station;
    }
    void Setweather_station(std::string val)
    {
        m_weather_station = val;
    }
    std::string Getstation_id()
    {
        return m_station_id;
    }
    void Setstation_id(std::string val)
    {
        m_station_id = val;
    }

    int GetYear()
    {
        return m_Year;
    }
    void SetYear(int val)
    {
        m_Year = val;
    }
    uint8_t GetMonth()
    {
        return m_Month;
    }
    void SetMonth(uint8_t val)
    {
        m_Month = val;
    }
    uint8_t GetDay()
    {
        return m_Day;
    }
    void SetDay(uint8_t val)
    {
        m_Day = val;
    }

    void clear_max_min_tide_level()
    {
        max_tide_level  = -100.00;
        min_tide_level = 100.00;
    }


protected:
private:
    // tidesandcurrents.noaa.gov/noaacurrents/DownloadPredictions?fmt=csv&d=2015-01-26&id=FLK1301_4
    std::string m_weather_station ;
    std::string m_station_id ;
    std::string UNITS;
    std::string FORMAT ;
    std::string DATUM ;
    std::string TIME_ZONE ;
    std::string WEATHER_STATION ;
    std::string API;
    std::string BASE_URL;
    double max_tide_level;
    double min_tide_level;
    double last_tide_level;
    uint8_t event_day_data;
    int event_minute;//time(mil)minute
    int event_hour;//time(mil) hour
    double event_level_data;// rate
    int m_Year;
    uint8_t m_Month;
    uint8_t m_Day;
};

#endif // CTIDE_STATION_H
