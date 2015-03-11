#ifndef XB_DATA_H
#define XB_DATA_H
#include "Arduino.h"
#include <SoftwareSerial.h>



class xb_data
{
    public:
        xb_data();

        ~xb_data();

        /*Creates a checksum by adding temperature ,humidity ,wind_speed and ,wind_direction
        returns the checksum int or -1 if the checksum int == 0 to prevent empty checksums from passing
        */
        int create_checksum(int temperature,int humidity,int wind_speed,int wind_direction);

        /*
        Checks a checksum by adding the perature ,humidity ,wind_speed and ,wind_direction and comparing it to the checksum
        */
        bool check_checksum(int temperature,int humidity,int wind_speed,int wind_direction, int checksum);

        /*
        parses an int out of a csv string
        returns the parsed int and modifies string_offset by reference
        */
        int parse_CSV(SoftwareSerial &XBee)
        {
            String int_string = "";
          if(XBee.available())
          {
              if(Xbee.peek != ',')
              {
                int_string += Xbee.read();
              }
          }
          else
          {
              return -255;
          }
          if(XBee.available())
          {
              if(Xbee.peek != ',')
              {
                int_string += Xbee.read();
              }
          }
          else
          {
              return -255;
          }

          return(int_string.toInt());

        }

        /*
        Reads the incomming bytes from the XBee and parses them into their respective fields
        returns 1 on success -1 if the checksum is bad and -2 if the data is bad
        */
        int read_incomming(SoftwareSerial &XBee);

        /*
        Creates a string that will be sent via the XBee one char at a time

        the string follows this format
        @,temperature,humidity,wind_speed,wind_direction,checksum,^

        the sending delay per char is set in the constructor
        */
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
        uint8_t m_tide_level;
        uint8_t m_tide_rising_or_falling;

        char weather_start_char;
        char tide_start_char;
        int send_delay;
        String datas_string;

};

#endif // XB_DATA_H
