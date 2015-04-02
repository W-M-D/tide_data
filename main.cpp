#include <iostream>
#include "CTIDE_STATION.h"
using namespace std;
string weather_station = "8727520";
CTIDE_STATION tide_station(0,weather_station);
int main()
{
    tide_station.fetch_predictive_tide_data_day();
    tide_station.fetch_recent_predictive_tide_data();
    tide_station.print_event_data();
    return 0;
}
