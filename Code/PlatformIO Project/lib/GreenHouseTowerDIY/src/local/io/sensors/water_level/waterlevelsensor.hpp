/*
 WaterlevelSensor.h - ESP32GreenHouseDIY Distance Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef WATERLEVELSENSOR_HPP
#define WATERLEVELSENSOR_HPP
#include <HCSR04.h>
#include <functional>
#include <memory>

#include <local/io/sensors/temperature/towertemp.hpp>
#include <utilities/network_utilities.hpp>

class WaterLevelSensor {
 private:
  // Private variables
  double _radius;
  double _height;
  struct Data_t {
    double water_level;
    double water_level_percentage;
  };

  TowerTemp* _towerTemp;
  std::shared_ptr<UltraSonicDistanceSensor>
      _distanceSensor;  // Initialize sensor that uses digital pins 13 and 12.
  // Private functions
  double readSensor();

 public:
  // Constructor
  WaterLevelSensor(TowerTemp* _towerTemp);
  virtual ~WaterLevelSensor();
  void begin();
  // Read the water level
  Data_t readWaterLevelUltraSonic();
  Data_t result;
};
#endif
