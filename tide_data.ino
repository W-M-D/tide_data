/*
  Repeating Wifi Web client

 This sketch connects to a a web server and makes a request
 using an Arduino Wifi shield.

 Circuit:
 * Wifi shield attached to pins 10, 11, 12, 13

 created 23 April 2012
 modifide 31 May 2012
 by Tom Igoe

 http://arduino.cc/en/Tutorial/WifiWebClientRepeating
 This code is in the public domain.
 */

#include <SPI.h>
#include <WiFi.h>
#include "CTIDE_STATION.h"
char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password

// Initialize the Wifi client library
WiFiClient client;
// server address:
//IPAddress server(64,131,82,241);


void setup() {
int status = WL_IDLE_STATUS;

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println(F("WiFi shield not present"));
    // don't continue:
    while(true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print(F("Attempting to connect to SSID: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(15000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();

}

    CTIDE_STATION tide_data(25000,"8727520");

boolean first_pass = true;
void loop() {
     int year = 2015;
     int month = 02;
     int day = 16;

     if (first_pass)
     {

     tide_data.fetch_predictive_tide_data_day(client ,day,month,year);

     first_pass = false;
     }
     else
     {
        tide_data.fetch_recent_predictive_tide_data(client);
     }
    if(tide_data.parse_tide_data(client))
    {
      tide_data.print_event_data();
    }

}

// this method makes a HTTP connection to the server:


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F("signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}






