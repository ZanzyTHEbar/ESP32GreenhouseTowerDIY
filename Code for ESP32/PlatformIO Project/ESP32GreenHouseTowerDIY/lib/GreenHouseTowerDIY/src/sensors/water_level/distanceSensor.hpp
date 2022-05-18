/*
 distanceSensor.h - ESP32GreenHouseDIY Distance Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef HCSR04_HPP
#define HCSR04_HPP
#include <defines.hpp>
#include <HCSR04.h>

class DistanceSensor
{
public:
  // Constructor
  DistanceSensor();
  virtual ~DistanceSensor();

  // Initialize the library
  double readSensor();
  double readWaterLevel();
  void indicateWaterLevel();


private:
  // Private variables
};

extern DistanceSensor distanceSensor;
#endif