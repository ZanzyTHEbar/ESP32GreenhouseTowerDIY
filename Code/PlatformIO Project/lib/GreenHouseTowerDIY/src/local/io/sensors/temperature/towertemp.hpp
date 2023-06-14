/*
 TowerTemp.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef TOWERTEMP_HPP
#define TOWERTEMP_HPP
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <vector>
#include "local/data/visitor.hpp"

using Temp_Array_t = std::vector<float>;
class TowerTemp : public Element<Visitor<SensorInterface<Temp_Array_t>>>,
                  public SensorInterface<Temp_Array_t> {
  std::string printAddress(DeviceAddress deviceAddress, size_t size);
  int _sensors_count;
  // Setup a oneWire instance to communicate with any OneWire devices
  OneWire oneWire;
  // Pass our oneWire reference to Dallas Temperature.
  DallasTemperature sensors;
  // variable to hold device addresses
  DeviceAddress temp_sensor_addresses;
  size_t size;

  void readAddresses();

 public:
  TowerTemp();
  virtual ~TowerTemp();
  bool begin();
  void checkSensors();
  void setSensorCount();
  int getSensorCount();

  std::vector<float> read() override;
  const std::string& getSensorName() override;
  void accept(Visitor<SensorInterface<Temp_Array_t>>& visitor) override;

  Temp_Array_t temp_sensor_results;
  Temp_Array_t getTempC();
  Temp_Array_t getTempF();
};
#endif