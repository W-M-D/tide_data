#include <iostream>
#include "CTIDE_STATION.h"
#include "CUART_SERIAL.h"
using namespace std;
string Virginia_Key_FL_ID = "8723214";
string Cutler_FL_ID = "8723289";
string Maimi_marina_FL_ID = "8723165";
string Key_Biscayne_FL_ID = "8723232";
string Boca_chita_FL_ID = "8723355";
CTIDE_STATION Virginia_Key_FL(0,Virginia_Key_FL_ID);
CTIDE_STATION Cutler(0,Cutler_FL_ID);
CTIDE_STATION Maimi_marina_FL(0,Maimi_marina_FL_ID);
CTIDE_STATION Key_Biscayne_FL(0,Key_Biscayne_FL_ID);
CTIDE_STATION Boca_chita_FL(0,Boca_chita_FL_ID);
CUART_SERIAL Serial(3);
int main()
{
    Virginia_Key_FL.fetch_predictive_tide_data_day();
    Virginia_Key_FL.fetch_recent_predictive_tide_data();


    Cutler.fetch_predictive_tide_data_day();
    Cutler.fetch_recent_predictive_tide_data();

   Maimi_marina_FL.fetch_predictive_tide_data_day();
   Maimi_marina_FL.fetch_recent_predictive_tide_data();

   // Key_Biscayne_FL.fetch_predictive_tide_data_day();
  //  Key_Biscayne_FL.fetch_recent_predictive_tide_data();

    Boca_chita_FL.fetch_predictive_tide_data_day();
    Boca_chita_FL.fetch_recent_predictive_tide_data();

    Virginia_Key_FL.print_event_data();
    Cutler.print_event_data();
    Maimi_marina_FL.print_event_data();
    //Key_Biscayne_FL.print_event_data();
    Boca_chita_FL.print_event_data();

    std::string sendstring = "cutler,";
    sendstring += Cutler.tide_rising_or_falling();
    sendstring += ',';
    sendstring += Cutler.tide_percent_level();

    return 0;
}
