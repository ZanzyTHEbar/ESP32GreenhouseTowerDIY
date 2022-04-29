/*
 HMS.h - HBAT library
 Copyright (c) 2021 Zacariah Austin Heim.
 */

#ifndef HMS_h
#define HMS_h
#include "defines.hpp"
#include <ACS712.h>

class HMS
{
public:
  HMS();
  virtual ~HMS();
  void calibrateAmps();
  void setupSensor();
  int readAmps();
  float readVoltage(int pinnumber);
  float *readSensAndCondition();
  String generateDeviceID();

private:
};

extern HMS HMSmain;

#endif