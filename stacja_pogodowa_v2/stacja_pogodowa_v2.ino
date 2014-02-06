#include <Wire.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>  

#include "WeatherStation.h"

WeatherStation station;

void setup() {

 Serial.begin(9600); 
 Serial.println(F("Start!"));
 station.begin(); 
}

void loop() {
  station.readSensors();
  Serial.print(station.getHumidity());
  Serial.print(F(","));
  Serial.print(station.getTemp());
  Serial.print(F(","));
  Serial.print(station.getPressure());
  Serial.print(F(","));
  Serial.println(station.getLight());
  delay(6000);
};
