/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef PHSENSOR_HPP
#define PHSENSOR_HPP
#include <ph_grav.h>
#include <string>
#include <utilities/network_utilities.hpp>

class PHSENSOR {
 public:
  // Constructor
  PHSENSOR();
  virtual ~PHSENSOR();
  void begin();
  void readPH();
  void eventListener(std::string result);
  void parse_cmd(std::string string);

  struct PH_t {
    float ph;
  };

  PH_t phContainer;

  const PH_t* getPH();
  void setPH();

  Gravity_pH pH;
  // Variables
  std::string _pHTopic;
  std::string _pHOutTopic;

 private:
  // Private functions
  void serialEvent();

  int _phDnPIN;
  int _phUpPIN;
  int _doseTimeSm;
  int _doseTimeMed;
  int _doseTimeLg;
  std::string _inputstring;        // a string to hold incoming data from the PC
  boolean _input_string_complete;  // a flag to indicate have we received all
                                   // the data from the PC
};
#endif  // PHSENSOR_HPP
