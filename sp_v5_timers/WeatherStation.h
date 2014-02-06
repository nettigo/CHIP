typedef struct WeatherStation {
  float humidity, temp, pressure;
  int light;
  unsigned long t_humidity, t_temp, t_pressure, t_light;
} WeatherStation;

WeatherStation station;


/*
* Przygotowujemy czujniki do działania i wpisujemy wartości do zmiennych
*/
void init_sensors(void){
  if(!bmp.begin())    
  {
    Serial.print(F("Problem z odczytem BMP085!"));
    while(1);
  }
  dht.begin();
  station.light 	= -99;
  station.humidity 	= -99;
  station.temp		= -99;
  station.pressure	= -99;

};


/*
* Jedna zbiorcza funkcja do odczytu wszystkich danych z czujników
*/
void readSensorsData(void) {
  station.humidity = dht.readHumidity();
  station.t_humidity= millis();

  station.temp = dht.readTemperature();
  station.t_temp = millis();

  if (isnan(station.temp)) { 
    station.temp = -99; 
  };
  if (isnan(station.humidity)) { 
    station.humidity = -99; 
  };

  sensors_event_t event;

  bmp.getEvent(&event);
  if (event.pressure){ 
    station.pressure = event.pressure;
    station.t_pressure = millis();

  } 
  else {
    station.pressure = -99;
  };

  station.light = analogRead(A3);
  station.t_light = millis();
}


/*
* Wyślij dane z czujników na Serial
*/


void logSensorsData(void) {
  Serial.print(station.humidity);
  Serial.print(F(","));
  Serial.print(station.temp);
  Serial.print(F(","));
  Serial.print(station.pressure);
  Serial.print(F(","));
  Serial.println(station.light);
};


