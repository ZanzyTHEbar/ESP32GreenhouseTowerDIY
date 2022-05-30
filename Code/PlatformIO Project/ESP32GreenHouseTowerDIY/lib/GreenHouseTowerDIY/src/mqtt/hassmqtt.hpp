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

#if ENABLE_MDNS_SUPPORT
  int DiscovermDNSBroker();
#endif // ENABLE_MDNS_SUPPORT

  int pump_relay_pin;
  char *pHTopic;
  char *pHOutTopic;
  int phDnPIN;
  int phUpPIN;
  int doseTimeSm;
  int doseTimeMed;
  int doseTimeLg;

private:
};

extern HASSMQTT hassmqtt;
#endif // HAMQTT_HPP