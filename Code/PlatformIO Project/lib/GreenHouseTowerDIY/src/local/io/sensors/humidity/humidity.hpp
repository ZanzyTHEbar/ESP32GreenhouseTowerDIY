/*
 Humidity.hpp - ESP32GreenHouseDIY Humidity library
 Copyright (c) 2021 ZanzyTHEbar
 */
#ifndef HUMIDITY_HPP
#define HUMIDITY_HPP

#include <Adafruit_SHT31.h>
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <functional>
#include <unordered_map>
#include "local/data/config/config.hpp"
#include "local/data/visitor.hpp"

// #define DHTTYPE DHT11  // DHT 11
// #define DHTTYPE DHT22  // DHT 22 (AM2302)
// #define DHTTYPE DHT21  // DHT 22 (AM2302)

using Humidity_Return_t = std::unordered_map<std::string, float>;

class Humidity : public Element<Visitor<SensorInterface<Humidity_Return_t>>>,
                 public SensorInterface<Humidity_Return_t> {
  enum _HUMIDITY_SENSORS_ACTIVE {
    HUMIDITY_SENSORS_ACTIVE_NONE,
    HUMIDITY_SENSORS_ACTIVE_SHT31,
    HUMIDITY_SENSORS_ACTIVE_SHT31_2,
    HUMIDITY_SENSORS_ACTIVE_BOTH,
    HUMIDITY_SENSORS_ACTIVE_DHT,
    HUMIDITY_SENSORS_ACTIVE_DHT_SHT31,
    HUMIDITY_SENSORS_ACTIVE_DHT_SHT31_2,
  };
  uint32_t _delayS;
  bool _enableHeater;
  int _loopCnt;
  Humidity_Return_t _humidity;
  static std::unordered_map<_HUMIDITY_SENSORS_ACTIVE, std::string>
      humidity_sensors_map;
  static std::string map_return_keys[5];
  _HUMIDITY_SENSORS_ACTIVE _humiditySensorsActive;

  GreenHouseConfig& _config;
  Adafruit_SHT31 sht31;
  Adafruit_SHT31 sht31_2;
  DHT_Unified dht;

  _HUMIDITY_SENSORS_ACTIVE setup();
  void readDHT();
  void readSHT31();

  bool checkHeaterEnabled();
  float towerHumidity();
  float towerTemp();

 public:
  Humidity(GreenHouseConfig& config);
  virtual ~Humidity();
  void begin();
  Humidity_Return_t read() override;
  const std::string& getSensorName() override;
  void accept(Visitor<SensorInterface<Humidity_Return_t>>& visitor) override;
};
#endif