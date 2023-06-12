/*
 basicmqtt.hpp - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BASEMQTT_HPP
#define BASEMQTT_HPP
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

//*  Data Includes
#include "local/data/AccumulateData/accumulatedata.hpp"

/**
 * @brief MQTT Class
 */
class BaseMQTT : public IPAddress, public PubSubClient {
  WiFiClient& _espClient;
  AccumulateData& _accumulateData;
  GreenHouseConfig& _deviceConfig;

  // Private variables
  const long _interval;
  unsigned long _previousMillis;

 public:
  // Constructor
  BaseMQTT(WiFiClient& espClient,
           AccumulateData& data,
           GreenHouseConfig& config);
  virtual ~BaseMQTT();

  void begin();
  void mqttLoop();
  void mqttReconnect();
  void subscribeAll();
  void publishAll();
  void mqttCallback(char* topic, byte* payload, unsigned int length);
};
#endif  // HAMQTT_HPP
