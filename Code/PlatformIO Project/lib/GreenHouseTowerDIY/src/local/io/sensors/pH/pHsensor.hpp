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

class PHSENSOR
{
public:
  // Constructor
  PHSENSOR();
  virtual ~PHSENSOR();
  void begin();
  void phSensorLoop();
  void eventListener(const char *topic,
                     const uint8_t *payload,
                     uint16_t length);
  void parse_cmd(const char *string);

  struct PH_t
  {
    float ph;
  };

  PH_t phContainer;

  const PH_t *getPH();
  void setPH();

  // Friends
  friend class BASEMQTT;
  friend void onMqttMessage(const char *topic,
                            const uint8_t *payload,
                            uint16_t length);
  friend void callback(char *topic, uint8_t *payload, unsigned int length);
  friend void onMqttConnected();
  friend class HASSMQTT;

  Gravity_pH pH;

private:
  // Private functions
  void serialEvent();

  // Private variables
  const char *_pHTopic;
  const char *_pHOutTopic;
  int _phDnPIN;
  int _phUpPIN;
  int _doseTimeSm;
  int _doseTimeMed;
  int _doseTimeLg;
  std::string _inputstring;       // a string to hold incoming data from the PC
  boolean _input_string_complete; // a flag to indicate have we received all
                                  // the data from the PC
};
#endif // PHSENSOR_HPP
