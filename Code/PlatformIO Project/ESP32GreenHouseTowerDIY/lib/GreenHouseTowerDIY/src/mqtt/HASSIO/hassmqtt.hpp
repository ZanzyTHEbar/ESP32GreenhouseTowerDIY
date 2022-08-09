/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef HAMQTT_HPP
#define HAMQTT_HPP
#include <ArduinoHA.h>
#include "mqtt/base.hpp"

class HASSMQTT : public BaseMQTT
{
public:
  // Constructor
  HASSMQTT();
  virtual ~HASSMQTT();

  bool begin();
  void mqttLoop();

  // Friends
  friend void onMqttConnected();

private:
  // Private functions

  // Private variables
  unsigned long lastReadAt;
  unsigned long lastAvailabilityToggleAt;
  bool lastInputState;
  unsigned long lastSentAt;

  const PHSENSOR::ph_Data_t &phData = phsensor.ph_data.at("id");
};

extern HASSMQTT hassmqtt;
#endif // HAMQTT_HPP