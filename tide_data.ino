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
#include "CFETCH_TIDE_DATA.h"
char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
CFETCH_TIDE_DATA tide_data(25000);

// Initialize the Wifi client library
WiFiClient client;

// server address:
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
boolean lastConnected = false;                  // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while(true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(15000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {
     int year = 2015;
     int month = 01;
     int day = 30;
     String station = "FLK1301_4";

    tide_data.fetch_tide_data(client,station,year);
    tide_data.parse_tide_data(client);
    tide_data.print_event_data();

}

// this method makes a HTTP connection to the server:


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}






