void init_sensors(void){
  if(!bmp.begin())
  {
    Serial.print(F("Problem z odczytem BMP085!"));
    while(1);
  }
  dht.begin();
  light = -99;
  humidity = -99;
  pressure = -99;
  temp = -99;
};

void readSensorsData(void) {
  humidity = dht.readHumidity();
  temp = dht.readTemperature();

  if (isnan(temp)) { 
    temp = -99; 
  };
  if (isnan(humidity)) { 
    humidity = -99; 
  };

  sensors_event_t event;

  bmp.getEvent(&event);
  if (event.pressure){ 
    pressure = event.pressure;
  } 
  else {
    pressure = -99;
  };
  
  light = analogRead(A3);
}

void logSensorsData(void) {
  Serial.print(humidity);
  Serial.print(F(","));
  Serial.print(temp);
  Serial.print(F(","));
  Serial.print(pressure);
  Serial.print(F(","));
  Serial.println(light);
};

