/*
 TowerTemp.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef TOWERTEMP_HPP
#define TOWERTEMP_HPP
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <memory>
#include <vector>

class TowerTemp {
 public:
  TowerTemp();
  virtual ~TowerTemp();
  bool begin();
  void checkSensors();
  void setSensorCount();
  int getSensorCount();

  struct Temp {
    std::vector<float> temp;
  };

  Temp temp_sensor_results;

  Temp getTempC();
  Temp getTempF();

 private:
  void readAddresses();
  std::string printAddress(DeviceAddress deviceAddress, size_t size);

 private:
  int _sensors_count;
  // Setup a oneWire instance to communicate with any OneWire devices
  OneWire oneWire;
  // Pass our oneWire reference to Dallas Temperature.
  DallasTemperature sensors;
  // variable to hold device addresses
  DeviceAddress temp_sensor_addresses;
  size_t size;
};
#endif