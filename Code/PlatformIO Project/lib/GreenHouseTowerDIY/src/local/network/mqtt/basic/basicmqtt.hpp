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
#include "local/data/config/config.hpp"
#include "local/data/visitor.hpp"

/**
 * @brief MQTT Class
 */
class BaseMQTT : public IPAddress, public PubSubClient {
  WiFiClient& _espClient;
  GreenHouseConfig& _deviceConfig;

 public:
  BaseMQTT(WiFiClient& espClient, GreenHouseConfig& config);
  virtual ~BaseMQTT();

  void begin();
  void mqttLoop();
  void mqttReconnect();
  void mqttCallback(char* topic, byte* payload, unsigned int length);

  //* Data Handlers
  void dataHandler(const std::string& topic, const std::string& payload);
  void dataHandler(const std::string& topic, float payload);
  void dataHandler(const std::string& topic, std::vector<float> payload);
  void dataHandler(const std::string& topic, std::vector<std::string> payload);
  void dataHandler(const std::string& topic,
                   std::unordered_map<std::string, float> payload);

  bool networkConnected;
};
#endif  // HAMQTT_HPP
