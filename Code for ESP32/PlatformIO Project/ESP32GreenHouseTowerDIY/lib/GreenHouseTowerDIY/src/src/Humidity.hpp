/*
 Humidity.hpp - HBAT Humidity library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef HUMIDITY_HPP
#define HUMIDITY_HPP
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include "defines.hpp"

struct Hum
{
  float temp;
  float humidity;
  float temp_2;
  float humidity_2;
};

extern Hum result;

class Humidity
{
public:
  // Constructor
  Humidity();
  virtual ~Humidity();
  // Initialize the library
  int setupSensor();
  bool checkHeaterEnabled();
  float StackHumidity();
  float AverageStackTemp();
  Hum ReadSensor();
  int loopSFM3003();
  uint8_t crc8(const uint8_t data, uint8_t crc);

  // Variables
private:
};
extern Humidity humidity;
#endif