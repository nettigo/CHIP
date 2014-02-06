#include <SPI.h>
#include <Wire.h>
#include <DHT.h>

#include <Ethernet.h>
#include <Timers.h> 

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>  

Adafruit_BMP085 bmp = Adafruit_BMP085();
DHT dht(8, DHT22);

#include "WeatherStation.h"

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xA5, 0xB0 };  
byte ip[] = { 192, 168, 1, 177 };    

EthernetServer server(80);

Timers<2>  timer;

//******************************************
//* obsługa sieci
//******************************************
void processNetwork() {
  //tymczasowa zmienna, którą przechowuje bieżący znak otrzymywany z przeglądarki
  char c;

  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: application/json"));
          client.println(F("Connection: close"));  // the connection will be closed after completion of the response
          client.println();

          client.print(F("fn('{\"temp\":{\"v\":"));
          client.print(station.temp); 
          client.print(F(",\"t\":")); 
          client.print(millis()-station.t_temp);
          client.print(F("},\"pressure\":{\"v\":"));
          client.print(station.pressure);
          client.print(F(",\"t\":")); 
          client.print(millis()-station.t_pressure);
          client.print(F("},\"humidity\":{\"v\":"));
          client.print(station.humidity); 
          client.print(F(",\"t\":")); 
          client.print(millis()-station.t_humidity);
          client.print(F("},\"light\":{\"v\":"));
          client.print(station.light);
          client.print(F(",\"t\":")); 
          client.print(millis()-station.t_light);
          client.print(F("},\"time\":")); 
          client.print(millis());
          client.println(F("}')"));


          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }  //if (client.available())
    } // while (client.connected())
    // give the web browser time to receive the data
    delay(2);
    client.stop();
    Serial.println("client disconnected");
  };  //if (client)
};

//***********************************************************************
//***********************************************************************
void setup() {

  Serial.begin(9600); 
  Serial.println(F("Start!"));
  init_sensors();
  readSensorsData();
  Ethernet.begin(mac, ip);
  timer.attach(0,10000,readSensorsData);
  timer.attach(1,60000,logSensorsData);
}

//***********************************************************************
//***********************************************************************

void loop() {
  timer.process();
  processNetwork();
};


