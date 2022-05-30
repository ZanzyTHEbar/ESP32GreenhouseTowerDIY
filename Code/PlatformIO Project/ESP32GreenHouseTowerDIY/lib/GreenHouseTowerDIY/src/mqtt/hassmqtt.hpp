/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef HAMQTT_HPP
#define HAMQTT_HPP
#include <defines.hpp>
#include <ArduinoHA.h>
#if ENABLE_MDNS_SUPPORT
#include <ESPmDNS.h>
#endif // ENABLE_MDNS_SUPPORT

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

#if ENABLE_MDNS_SUPPORT
  int DiscovermDNSBroker();
#endif // ENABLE_MDNS_SUPPORT

private:
  // Private functions

  // Private variables
  int pump_relay_pin;
  char *pHTopic;
  char *pHOutTopic;
  int phDnPIN;
  int phUpPIN;
  int doseTimeSm;
  int doseTimeMed;
  int doseTimeLg;
};

extern HASSMQTT hassmqtt;
#endif // HAMQTT_HPP