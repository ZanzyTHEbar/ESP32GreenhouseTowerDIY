/*
 bh1750.hpp - ESP32GreenHouseDIY  bh1750 Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BH1750_HPP
#define BH1750_HPP
#include <Arduino.h>
#include <hp_BH1750.h>

class BH1750
{
public:
  // Constructor
  BH1750();
  virtual ~BH1750();

  // Initialize the library
  void begin();
  float getLux();

private:
  hp_BH1750 BH1750_sensor; // create the sensor object
  // Private variables
};
#endif
