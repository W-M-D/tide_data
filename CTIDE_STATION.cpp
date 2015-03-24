#include "CTIDE_STATION.h"
//1.0.6
CTIDE_STATION::CTIDE_STATION(int update_delay,std::string weather_station)
{
    WEATHER_STATION += "station=";
    WEATHER_STATION += weather_station;
    WEATHER_STATION += "&";

    UNITS=  "units=metric&";
    DATUM =  "datum=MLLW&";
    TIME_ZONE = "time_zone=GMT&";
    FORMAT = "format=csv";
    API = "/api/datagetter?";
    BASE_URL = "www.tidesandcurrents.noaa.gov";

    clear_max_min_tide_level();
}

int CTIDE_STATION::fetch_recent_predictive_tide_data()
{
    std::stringstream URL_string;
    URL_string << API << "product=predictions&" << "range=1&" << DATUM << WEATHER_STATION << TIME_ZONE << UNITS << FORMAT;
    send_string(URL_string.str());

    return 1;

}



int CTIDE_STATION::fetch_predictive_tide_data_day()
{
    std::stringstream URL_string;
    URL_string << API << "product=water_level&" << "date=today&" << DATUM << WEATHER_STATION << TIME_ZONE << UNITS << FORMAT;
    send_string(URL_string.str());
    return 1;
}

bool CTIDE_STATION::send_string(const std::string & tide_URL)
{
    CURL * curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string tide_data;
    std::string sending_string = "";
    sending_string.append(BASE_URL);
    sending_string.append(tide_URL);
    unsigned int options;
    if(curl)
    {
        curl_easy_setopt(curl,CURLOPT_NOSIGNAL ,1L );
        curl_easy_setopt(curl,CURLOPT_FAILONERROR,1L );
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tide_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_URL, sending_string.c_str());
        /* Perform the request, res will get the return code */

        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            return false;
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        parse_tide_data(tide_data);
        return true;
    }
    /* always cleanup */
    curl_easy_cleanup(curl);
    return false;
}


int CTIDE_STATION::parse_tide_data(std::string tide_data)
{

    int lines = 0;
    char newline = '\n';
    char startyear = '2';
    std::cout << tide_data <<std::endl;

    for(unsigned int offset; offset < tide_data.length(); offset++)
    {
        std::cout <<  offset <<tide_data.at(offset) <<std::endl;
        if(tide_data.at(offset)==newline)
        {
                lines++;
                std::cout << lines << std::endl;
                if(startyear == tide_data.at(offset + 1))
                {
                std::cout <<"First line" <<std::endl;
                offset = tide_data.find_first_of(' ',offset);
                parse_time(tide_data,offset);
                offset = tide_data.find_first_of(',',offset + 1);


                event_level_data = stof(tide_data,&offset);

                if(event_level_data > max_tide_level)
                {
                    max_tide_level = event_level_data;
                }
                if(event_level_data != 0.0 && event_level_data < min_tide_level)
                {
                    min_tide_level = event_level_data;
                }
                }
        }
    }

}

void CTIDE_STATION::print_event_data()
{
    std::cout << "Day , time(24hr) , level" << std::endl;
    std::cout << "*******************************" << std::endl;

    std::cout << (event_day_data);

    std::cout << (" , ");
    if(event_hour < 10)
    {
        std::cout << ("0");
    }
    std::cout << (event_hour);//day,time(mil),
    std::cout << (":");
    if(event_minute < 10)
    {
        std::cout << ("0");
    }

    std::cout << event_minute << " , " << event_level_data << std::endl;

    std::cout << "Max tide level: " << max_tide_level << std::endl;

    std::cout << "Min tide level: " << min_tide_level << std::endl;

    std::cout << "Last tide level:" << last_tide_level << std::endl;

    std::cout << "Tide %:" << tide_percent_level() << std::endl;

    std::cout << GetYear() << '-' << GetMonth() << '-' << GetDay() << std::endl;
    if(tide_rising_or_falling() == 0)
    {
        std::cout << "Tide is falling!" << std::endl;
    }
    else if(tide_rising_or_falling() == 1)
    {
        std::cout << "Tide is rising!" << std::endl;
    }
    else
    {
        std::cout << "Tide unchanged since last reading!" << std::endl;
    }

    std::cout << "*******************************" << std::endl;
}

CTIDE_STATION::~CTIDE_STATION()
{
    //dtor
}



