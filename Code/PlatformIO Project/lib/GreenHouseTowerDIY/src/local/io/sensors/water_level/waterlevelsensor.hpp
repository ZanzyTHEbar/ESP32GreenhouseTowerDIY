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
#include "local/data/visitor.hpp"
#include "local/io/sensors/temperature/towertemp.hpp"

class WaterLevelSensor : public Element<Visitor<SensorInterface<float>>>,
                         public SensorInterface<float> {
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
  double volume();
  //* Read the water level
  float read() override;
  //* Accept the visitor
  const std::string& getSensorName() override;
  void accept(Visitor<SensorInterface<float>>& visitor) override;
};

class WaterLevelPercentage : public Element<Visitor<SensorInterface<float>>>,
                             public SensorInterface<float> {
  WaterLevelSensor& _waterLevelSensor;

 public:
  WaterLevelPercentage(WaterLevelSensor& waterLevelSensor);
  float read() override;
  const std::string& getSensorName() override;
  void accept(Visitor<SensorInterface<float>>& visitor) override;
};

#endif
