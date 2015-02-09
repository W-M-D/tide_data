#ifndef XB_DATA_H
#define XB_DATA_H
#include "Arduino.h"
#include <SoftwareSerial.h>



class xb_data
{
    public:
        xb_data();

        ~xb_data();


        int create_checksum(int temperature,int humidity,int wind_speed,int wind_direction);
        bool check_checksum(int temperature,int humidity,int wind_speed,int wind_direction, int checksum);
        
        int parse_CSV(String data_string, int & string_offset)
        {
          int offset = 0; 
          String int_string = "";
          offset = data_string.indexOf(',',string_offset);
          string_offset = data_string.indexOf(',',offset + 1);
          for(int i = offset + 1; i <= string_offset; i ++)
          {
            int_string += data_string.charAt(i);
          }
          return int_string.toInt();
        }
        
        int read_incomming(SoftwareSerial &XBee);
        int send_data(SoftwareSerial &XBee);
        
        int Gettemperature() { return m_temperature; }
        void Settemperature(int val) { m_temperature = val; }
        int Gethumidity() { return m_humidity; }
        void Sethumidity(int val) { m_humidity = val; }
        int Getwind_speed() { return m_wind_speed; }
        void Setwind_speed(int val) { m_wind_speed = val; }
        int Getwind_direction() { return m_wind_direction; }
        void Setwind_direction(int val) { m_wind_direction = val; }
        
    protected:
    private:

        int m_temperature;//
        int m_humidity;
        int m_wind_speed;
        int m_wind_direction;
        char start_char;
        char end_char;
        int send_delay;
        String datas_string;

};

#endif // XB_DATA_H
