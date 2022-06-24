/*
 distanceSensor.h - ESP32GreenHouseDIY Distance Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef DISTANCESENSOR_HPP
#define DISTANCESENSOR_HPP
#include <defines.hpp>
#include <HCSR04.h>

class DistanceSensor
{
public:
  // Constructor
  DistanceSensor();
  virtual ~DistanceSensor();
  // Initialize the library
  void begin();
  // Read the sensor
  double readSensor();
  int readWaterLevel();
  void indicateWaterLevel();

private:
  double _radius;
  double _height;
  byte _leds[3];
  // Private variables
};

extern DistanceSensor distanceSensor;
#endif