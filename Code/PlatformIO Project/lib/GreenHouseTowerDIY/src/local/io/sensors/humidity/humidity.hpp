/*
 Humidity.hpp - ESP32GreenHouseDIY Humidity library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef HUMIDITY_HPP
#define HUMIDITY_HPP

#include <Arduino.h>
#include <Wire.h>
#include <functional>
#include <memory>
#include <unordered_map>
#if USE_SHT31_SENSOR
#include <Adafruit_SHT31.h>
#endif  // USE_SHT31_SENSOR

#if USE_DHT_SENSOR
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#if (USE_DHT11)
#define DHTTYPE DHT11  // DHT 11
#elif (USE_DHT22)
#define DHTTYPE DHT22  // DHT 22 (AM2302)
#elif (USE_DHT21)
#define DHTTYPE DHT21  // DHT 22 (AM2302)
#endif

#endif  // USE_DHT_SENSOR

class Humidity {
 public:
  Humidity();
  virtual ~Humidity();

  void begin();
  //* Initialize the library
  enum _HUMIDITY_SENSORS_ACTIVE {
    HUMIDITY_SENSORS_ACTIVE_NONE = 0,
    HUMIDITY_SENSORS_ACTIVE_SHT31 = 1,
    HUMIDITY_SENSORS_ACTIVE_SHT31_2 = 2,
    HUMIDITY_SENSORS_ACTIVE_BOTH = 3,
    HUMIDITY_SENSORS_ACTIVE_DHT = 4,
    HUMIDITY_SENSORS_ACTIVE_DHT_SHT31 = 5,
    HUMIDITY_SENSORS_ACTIVE_DHT_SHT31_2 = 6,
  };
  _HUMIDITY_SENSORS_ACTIVE _humiditySensorsActive;
  struct Hum {
    float temp;
    float humidity;
#if USE_SHT31_SENSOR
    float temp_2;
    float humidity_2;
    float humidity_sht31;
    float temp_sht31;
    float humidity_sht31_2;
    float temp_sht31_2;
#endif  // USE_SHT31_SENSOR
  };

  Hum result;

  _HUMIDITY_SENSORS_ACTIVE setup();
#if USE_DHT_SENSOR
  Hum readDHT();
#endif  // USE_DHT_SENSOR

#if USE_SHT31_SENSOR
  bool checkHeaterEnabled();
  float StackHumidity();
  float AverageStackTemp();
  Hum ReadSensor();
#endif  // USE_SHT31_SENSOR

  //* Variables
  static std::unordered_map<_HUMIDITY_SENSORS_ACTIVE, std::string>
      humidity_sensors_map;

 private:
  uint32_t _delayS;
#if USE_SHT31_SENSOR
  bool _enableHeater;
  int _loopCnt;
#endif  // USE_SHT31_SENSOR

//* Global Variables
#if USE_SHT31_SENSOR
  Adafruit_SHT31 sht31;
  Adafruit_SHT31 sht31_2;
#endif  // USE_SHT31_SENSOR

#if USE_DHT_SENSOR
  DHT_Unified dht;
#endif  // USE_DHT_SENSOR
};
#endif