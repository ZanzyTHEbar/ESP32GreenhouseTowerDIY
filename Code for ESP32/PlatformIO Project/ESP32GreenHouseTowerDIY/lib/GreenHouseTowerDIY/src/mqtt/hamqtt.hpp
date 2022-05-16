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
  /* bool MQTTSetup();
  bool MQTTLoop();
  int DiscovermDNSBroker();
  void RunMqttService();
  void MessageReceived(char topic[], char payload[]);
  void MQTTPublish(char topic[], char payload[]);
  bool ReConnect(); */

private:
};

extern HASSMQTT hassmqtt;
#endif // HAMQTT_HPP