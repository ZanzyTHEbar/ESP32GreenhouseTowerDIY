/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
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

  void setupDistanceSensor();
  double readWaterLevel();
  
  // Initialize the library
  

private:
  // Private variables
  double distance;
};

extern DistanceSensor distanceSensor;
#endif