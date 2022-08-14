/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef HASSMQTT_HPP
#define HASSMQTT_HPP
#include <ArduinoHA.h>
#include "mqtt/mqttbase.hpp"

class HASSMQTT
{
public:
  // Constructor
  HASSMQTT(Network *network,
           Config *deviceConfig,
           PUMP *pump,
           Relays *relays,
           AccumulateData *accumulateData,
           PHSENSOR *phsensor,
           NetworkNTP *ntp,
           XMqttBaseClass *baseMQTT,
           Humidity *humidity,
           TowerTemp *tower_temp,
           LDR *ldr);
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
  long lastReconnectAttempt;

  Network *network;
  Config *deviceConfig;
  PUMP *pump;
  Relays *relays;
  AccumulateData *accumulateData;
  PHSENSOR *phsensor;
  NetworkNTP *ntp;
  XMqttBaseClass *baseMQTT;
  Humidity *humidity;
  TowerTemp *tower_temp;
  LDR *ldr;
};
#endif // HAMQTT_HPP