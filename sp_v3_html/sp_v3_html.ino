#include <SPI.h>
#include <Wire.h>
#include <DHT.h>

#include <Ethernet.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>  

#include "WeatherStation.h"

WeatherStation station;
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xA5, 0xB0 };  
//the IP address for the shield:
byte ip[] = { 192, 168, 22, 177 };    
EthernetServer server(80);

void setup() {

 Serial.begin(9600); 
 Serial.println(F("Start!"));
 station.begin(); 
 Ethernet.begin(mac, ip);
}

void loop() {
  char c;
  station.readSensors();
  Serial.print(station.getHumidity());
  Serial.print(F(","));
  Serial.print(station.getTemp());
  Serial.print(F(","));
  Serial.print(station.getPressure());
  Serial.print(F(","));
  Serial.println(station.getLight());
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
          client.print(station.getTemp());
          client.print(F(", odczyt sprzed "));
          client.print(station.getTimeDelta(WS_TEMP_READING));
          client.println(F(" ms.<br />"));      


          client.println(F("Ciśnienie:"));
          client.print(station.getPressure());
          client.print(F(", odczyt sprzed "));
          client.print(station.getTimeDelta(WS_PRESSURE_READING));
          client.println(F(" ms.<br />"));      

          client.println(F("Wilgotność:"));
          client.print(station.getHumidity());
          client.print(F("%, odczyt sprzed "));
          client.print(station.getTimeDelta(WS_HUMIDITY_READING));
          client.println(F(" ms.<br />"));      

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
      }
    }
    // give the web browser time to receive the data
    delay(2);
    client.stop();
    Serial.println("client disconnected");
  }
};
