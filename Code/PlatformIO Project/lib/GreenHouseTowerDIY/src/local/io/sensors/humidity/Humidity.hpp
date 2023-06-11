/*
 Humidity.hpp - ESP32GreenHouseDIY Humidity library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef HUMIDITY_HPP
#define HUMIDITY_HPP
#include <Adafruit_SHT31.h>
#include <Arduino.h>
#include <Wire.h>
#include <functional>
#include <local/data/config/config.hpp>
#include <memory>
#include <unordered_map>
#include <utilities/network_utilities.hpp>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class Humidity {
 private:
  GreenHouseConfig& configManager;
  uint32_t _delayS;
  bool _enableHeater;
  int _loopCnt;

  // Global Variables
  std::shared_ptr<Adafruit_SHT31> sht31;
  std::shared_ptr<Adafruit_SHT31> sht31_2;
  std::shared_ptr<DHT_Unified> dht;

 public:
  // Constructor
  Humidity(GreenHouseConfig& config);
  virtual ~Humidity();

  void begin();
  // Initialize the library
  enum HUMIDITY_SENSORS_ACTIVE_e {
    HUMIDITY_SENSORS_ACTIVE_NONE = 0,
    HUMIDITY_SENSORS_ACTIVE_SHT31 = 1,
    HUMIDITY_SENSORS_ACTIVE_SHT31_2 = 2,
    HUMIDITY_SENSORS_ACTIVE_BOTH = 3,
    HUMIDITY_SENSORS_ACTIVE_DHT = 4,
    HUMIDITY_SENSORS_ACTIVE_DHT_SHT31 = 5,
    HUMIDITY_SENSORS_ACTIVE_DHT_SHT31_2 = 6,
  };

  struct Hum {
    float temp;
    float humidity;
    float temp_2;
    float humidity_2;
    float humidity_sht31;
    float temp_sht31;
    float humidity_sht31_2;
    float temp_sht31_2;
  };

  HUMIDITY_SENSORS_ACTIVE_e humiditySensorsActive;
  Hum result;

  HUMIDITY_SENSORS_ACTIVE_e setup();
  Hum readDHT();

  bool checkHeaterEnabled();
  float stackHumidity();
  float averageStackTemp();
  Hum readSHT31();

  // Variables
  static std::unordered_map<HUMIDITY_SENSORS_ACTIVE_e, std::string>
      humidity_sensors_map;
  static std::unordered_map<std::string, uint8_t> dht_types;
};
#endif
