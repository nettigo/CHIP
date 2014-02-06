#include <Wire.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <Ethernet.h>


Adafruit_BMP085 bmp = Adafruit_BMP085();
DHT dht(8, DHT22);

EthernetServer server(80);
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0xA5, 0xB0 };  
byte ip[] = { 
  192, 168, 22, 177 }; 

float humidity, pressure, temp;
int light;

#include "WeatherStation.h"

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
          // send a standard http response header
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println(F("Connection: close"));  // the connection will be closed after completion of the response
          client.println(F("Refresh: 30"));  // refresh the page automatically every 5 sec
          client.println();
          client.println(F("<!DOCTYPE HTML>"));
          client.println(F("<html><head>"));
          client.println(F("<meta content='text/html;charset=UTF-8' http-equiv='content-type'>"));
          client.println(F("</head><body>Dane odczytane:<br/>Temperatura:"));
          client.print(temp);
          client.println(F(" &deg;C .<br/>"));      


          client.println(F("Ciśnienie:"));
          client.print(pressure);
          client.println(F(" hpa.<br/>"));      

          client.println(F("Wilgotność:"));
          client.print(humidity);
          client.println(F(" %.<br />"));      

          client.println(F("Światło:"));
          client.print(light);
          client.println(F(".<br />"));      
          
          //korzystając z funkcji millis() wyświetl info ile minęło sekund od ostatniego startu/restartu Arduino.
          client.print(F("<small>"));
          client.print(millis()/1000.0);
          client.print(F(" s od startu Arduino.</small>"));
          client.println(F("</body></html>"));
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

void setup() {
  Serial.begin(9600); 
  init_sensors();
  Ethernet.begin(mac, ip);
  Serial.println(F("Start!"));
}


void loop() {
  readSensorsData();
  processNetwork();
  logSensorsData();
};


