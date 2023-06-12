/*
 WaterlevelSensor.h - ESP32GreenHouseDIY Distance Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef WATERLEVELSENSOR_HPP
#define WATERLEVELSENSOR_HPP
#include <HCSR04.h>
#include <functional>

#include <utilities/network_utilities.hpp>
#include "local/io/sensors/temperature/towertemp.hpp"

struct WaterLevelData_t {
  double water_level;
  double water_level_percentage;
};

class WaterLevelSensor : public SensorInterface<WaterLevelData_t> {
  //* Private variables
  double _radius;
  double _height;

  TowerTemp& _towerTemp;
  UltraSonicDistanceSensor _distanceSensor;
  //* Private functions
  double readSensor();

 public:
  //* Constructor
  WaterLevelSensor(TowerTemp& _towerTemp);
  virtual ~WaterLevelSensor();
  void begin();
  const std::string& getSensorName() override;
  //* Read the water level
  WaterLevelData_t read() override;
  WaterLevelData_t result;
};
#endif
