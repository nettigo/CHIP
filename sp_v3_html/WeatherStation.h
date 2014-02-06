#define DHTPIN               8
#define LIGHT_SENSOR         A0
#define WS_SENSOR_SIZE       4

enum sensorTypes
{
  WS_PRESSURE_READING    = 0,
  WS_TEMP_READING        = 1,
  WS_LIGHT_READING       = 2,
  WS_HUMIDITY_READING    = 3
};
  
class WeatherStation {
  public: 
    WeatherStation(void);
    byte           begin(void);
    float          getPressure(void);
    float          getTemp(void);
    unsigned       getLight(void);
    float          getHumidity(void);
    void           readSensors(void);
    void           readTemp(void);
    void           readPressure(void);
    void           readLight(void);
    void           readHumidity(void);
    unsigned long  getTimestamp(sensorTypes);
    unsigned long  getTimeDelta(sensorTypes);
  private:
    float           readings[WS_SENSOR_SIZE];
    unsigned long   timestamps[WS_SENSOR_SIZE];
    Adafruit_BMP085 bmp;
    DHT             dht;
};

WeatherStation::WeatherStation(void) {
  for(byte i=0; i<WS_SENSOR_SIZE;i++){
    readings[i] = 0;
    timestamps[i] = 0;
  }
};

byte WeatherStation::begin(void) {
  byte t = 0;
  bmp = Adafruit_BMP085(10085);
  dht = DHT(DHTPIN,DHT22);
  if(!bmp.begin()) {return -1; };
  dht.begin();
};
unsigned long WeatherStation::getTimestamp( sensorTypes sensor ) { return timestamps[sensor]; };
unsigned long WeatherStation::getTimeDelta( sensorTypes sensor ) { return millis() - getTimestamp(sensor); };
float WeatherStation::getPressure(void)   { return readings[WS_PRESSURE_READING]; };
float WeatherStation::getHumidity(void)   { return readings[WS_HUMIDITY_READING]; };
float WeatherStation::getTemp(void)       { return readings[WS_TEMP_READING]; };
unsigned WeatherStation::getLight(void)   { return (unsigned)readings[WS_LIGHT_READING]; };

//**************************
void WeatherStation::readHumidity(void) {
    float h;
 
  // wilgotność
  h = dht.readHumidity();
  if(!isnan(h)) {
    readings[WS_HUMIDITY_READING] = h;
    timestamps[WS_HUMIDITY_READING] = millis();
  }
}
//*************************

void WeatherStation::readTemp(void){
  float h;
  h = dht.readTemperature();
  if(!isnan(h)) {
    readings[WS_TEMP_READING] = h;
    timestamps[WS_TEMP_READING] = millis();
  }

};

//**********************************

void WeatherStation::readPressure(void){
  sensors_event_t event;

  bmp.getEvent(&event);  
  if(event.pressure) {
    readings[WS_PRESSURE_READING] = event.pressure;
    timestamps[WS_PRESSURE_READING] = millis();
  }

}
//************************************
void WeatherStation::readLight(void) {
  readings[WS_LIGHT_READING] = analogRead(LIGHT_SENSOR);
  timestamps[WS_LIGHT_READING] = millis();
};
//************************************
void WeatherStation::readSensors(void){
  readHumidity();
    //temperatura
  readTemp();  
  //ciśnienie
  readPressure();
  //natężenie światła
  readLight();
  

};



