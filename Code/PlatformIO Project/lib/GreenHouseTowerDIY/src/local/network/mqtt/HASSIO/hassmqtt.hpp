/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef HAMQTT_HPP
#define HAMQTT_HPP
#include <Arduino.h>
#include <ArduinoHA.h>

class HASSMQTT {
 public:
  // Constructor
  HASSMQTT();
  virtual ~HASSMQTT();

  void loadMQTTConfig();
  void mqttLoop();

  // Friends
  friend class LDR;
  friend void onMqttMessage(const char* topic,
                            const uint8_t* payload,
                            uint16_t length);
  friend void onMqttConnected();

 private:
  // Private functions

  // Private variables
  unsigned long lastReadAt;
  unsigned long lastAvailabilityToggleAt;
  bool lastInputState;
  unsigned long lastSentAt;
};
#endif  // HAMQTT_HPP
