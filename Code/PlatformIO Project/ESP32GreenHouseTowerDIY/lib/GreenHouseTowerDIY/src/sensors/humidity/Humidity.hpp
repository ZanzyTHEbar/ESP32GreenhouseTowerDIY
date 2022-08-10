/*
 Humidity.hpp - ESP32GreenHouseDIY Humidity library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef HUMIDITY_HPP
#define HUMIDITY_HPP
#include <defines.hpp>
#include <Wire.h>
#include <memory>
#if USE_SHT31_SENSOR
#include <Adafruit_SHT31.h>
#endif // USE_SHT31_SENSOR

#if USE_DHT_SENSOR
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

#if (USE_DHT11)
#define DHTTYPE DHT11 // DHT 11
#elif (USE_DHT22)
#define DHTTYPE DHT22 // DHT 22 (AM2302)
#elif (USE_DHT21)
#define DHTTYPE DHT22 // DHT 22 (AM2302)
#endif

#endif // USE_DHT_SENSOR

class Humidity
{
public:
  // Constructor
  Humidity();
  virtual ~Humidity();
  // Initialize the library
  enum _HUMIDITY_SENSORS_ACTIVE
  {
    HUMIDITY_SENSORS_ACTIVE_NONE = 0,
    HUMIDITY_SENSORS_ACTIVE_SHT31 = 1,
    HUMIDITY_SENSORS_ACTIVE_SHT31_2 = 2,
    HUMIDITY_SENSORS_ACTIVE_BOTH = 3,
    HUMIDITY_SENSORS_ACTIVE_DHT = 4,
    HUMIDITY_SENSORS_ACTIVE_DHT_SHT31 = 5,
    HUMIDITY_SENSORS_ACTIVE_DHT_SHT31_2 = 6,
  };
  _HUMIDITY_SENSORS_ACTIVE _humiditySensorsActive;
  struct Hum
  {
    float temp;
    float humidity;
    float temp_2;
    float humidity_2;
    float humidity_sht31;
    float temp_sht31;
    float humidity_sht31_2;
    float temp_sht31_2;
  };

  Hum result;

  _HUMIDITY_SENSORS_ACTIVE begin();
#if USE_DHT_SENSOR
  Hum readDHT();
#endif // USE_DHT_SENSOR

#if USE_SHT31_SENSOR
  bool checkHeaterEnabled();
  float StackHumidity();
  float AverageStackTemp();
  Hum ReadSensor();
#endif // USE_SHT31_SENSOR

  // Friends
  friend class Config;
  friend class AccumulateData;

  // Variables
private:
  uint32_t _delayS;
#if USE_SHT31_SENSOR
  bool _enableHeater;
  int _loopCnt;
#endif // USE_SHT31_SENSOR

// Global Variables
#if USE_SHT31_SENSOR
  std::shared_ptr<Adafruit_SHT31> sht31;
  std::shared_ptr<Adafruit_SHT31> sht31_2;
#endif // USE_SHT31_SENSOR

#if USE_DHT_SENSOR
  std::shared_ptr<DHT_Unified> dht;
#endif // USE_DHT_SENSOR

  Humidity::Hum result;
};
extern Humidity humidity;
#endif