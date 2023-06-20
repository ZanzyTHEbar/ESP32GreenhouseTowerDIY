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
  uint32_t _delayS;
  bool _enableHeater;
  int _loopCnt;
  Humidity_Return_t _humidity;
  static std::unordered_map<GreenHouseConfig::HumidityFeatures_t, std::string>
      humidity_sensors_map;
  static std::string map_return_keys[6];
  GreenHouseConfig::HumidityFeatures_t _humiditySensorsActive;

  GreenHouseConfig& _config;
  Adafruit_SHT31 sht31;
  Adafruit_SHT31 sht31_2;
  DHT_Unified dht;

  GreenHouseConfig::HumidityFeatures_t setup();
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
  void accept(
      Visitor<SensorInterface<Humidity_Return_t>>& visitor) override;
};
#endif