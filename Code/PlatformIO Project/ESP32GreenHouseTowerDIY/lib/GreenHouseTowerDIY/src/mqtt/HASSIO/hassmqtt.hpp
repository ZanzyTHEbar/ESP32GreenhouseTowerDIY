/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef HAMQTT_HPP
#define HAMQTT_HPP
#include <defines.hpp>
#include <ArduinoHA.h>

#include "network/network.hpp"
#include "network/ntp.hpp"
#include "io/Pump/pump.hpp"
#include "io/Relays/Relays.hpp"
#include "sensors/pH/pHsensor.hpp"
#include "data/StateManager/StateManager.hpp"
#include "data/AccumulateData/accumulatedata.hpp"

class HASSMQTT : public Network
{
public:
  // Constructor
  HASSMQTT();
  virtual ~HASSMQTT();

  void loadMQTTConfig();
  void mqttLoop();

  // Friends
  friend class LDR;
  friend void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length);
  friend void onMqttConnected();

private:
  // Private functions

  // Private variables
  unsigned long lastReadAt;
  unsigned long lastAvailabilityToggleAt;
  bool lastInputState;
  unsigned long lastSentAt;
};

extern HASSMQTT hassmqtt;
#endif // HAMQTT_HPP