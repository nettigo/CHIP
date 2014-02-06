#include <Wire.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>


Adafruit_BMP085 bmp = Adafruit_BMP085();
DHT dht(8, DHT22);

void setup() {

 Serial.begin(9600); 
 if(!bmp.begin())
  {
    Serial.print(F("Problem z odczytem BMP085!"));
    while(1);
  }
  Serial.println(F("Start!"));
 
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(t)) { t = -99; };
  if (isnan(h)) { h = -99; };

  float pressure;
  
  sensors_event_t event;

  bmp.getEvent(&event);
  if (event.pressure){ pressure = event.pressure;} else {pressure = -99;};


    Serial.print(h);
    Serial.print(F(","));
    Serial.print(t);
    Serial.print(F(","));
    Serial.print(pressure);
    Serial.print(F(","));
    Serial.println(analogRead(A3));
    
  delay(60000);
};
