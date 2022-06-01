/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef HAMQTT_HPP
#define HAMQTT_HPP
#include <defines.hpp>
#include <ArduinoHA.h>

class HASSMQTT
{
public:
  // Constructor
  HASSMQTT();
  virtual ~HASSMQTT();

  void loadMQTTConfig();
  void mqttSetup();
  void mqttLoop();

  // Friends
  friend class LDR;
  friend void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length);
  friend void onMqttConnected();

private:
  // Private functions

  // Private variables
};

extern HASSMQTT hassmqtt;
#endif // HAMQTT_HPP