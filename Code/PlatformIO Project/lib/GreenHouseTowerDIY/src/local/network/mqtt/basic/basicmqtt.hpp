/*
 basicmqtt.hpp - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BASEMQTT_HPP
#define BASEMQTT_HPP
#include <Arduino.h>
// #include <PubSubClient.h>
#include <ArduinoJson.h>
#include <MQTTClient.h>
#include "local/data/config/config.hpp"
#include "local/data/visitor.hpp"

/**
 * @brief MQTT Class
 */
class BaseMQTT : public IPAddress, public MQTTClientCallback {
  GreenHouseConfig& _deviceConfig;
  ProjectConfig& _projectConfig;
  MQTTClient* _client;

  //* Configuration for MQTT
  StaticJsonDocument<384> doc;
  JsonObject mqttConfig;

  //* Callbacks for MQTT library
  virtual void onConnected(MQTTClient* client) override;
  virtual void onDataReceived(MQTTClient* client,
                              const mqtt_client_event_data* data) override;
  virtual void onSubscribed(MQTTClient* thisClient,
                            const mqtt_client_topic_data* topic) override;
  virtual void onTopicUpdate(MQTTClient* client,
                             const mqtt_client_topic_data* topic) override;

 public:
  BaseMQTT(GreenHouseConfig& config,
           ProjectConfig& _projectConfig,
           MQTTClient* client);

  void begin();
  bool discovermDNSBroker();
  bool mqttConnected() { return _client->connected(); }

  //* Data Handlers
  void dataHandler(const std::string& topic, const std::string& payload);
  void dataHandler(const std::string& topic, float payload);
  void dataHandler(const std::string& topic, std::vector<float> payload);
  void dataHandler(const std::string& topic, std::vector<std::string> payload);
  void dataHandler(const std::string& topic,
                   std::unordered_map<std::string, float> payload);

  bool brokerDiscovery;
  unsigned long currentLoopMillis;
  unsigned long previousPublishMillis;
  unsigned long publishTime;
};
#endif  // HAMQTT_HPP
