/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef HAMQTT_HPP
#define HAMQTT_HPP
#include <Arduino.h>
#include <ArduinoHA.h>
#include <WiFi.h>
#include <functional>

//*  Data Includes
#include "local/data/AccumulateData/accumulatedata.hpp"

class HASSMQTT {
  WiFiClient& _espClient;
  
  //* Custom Class objects
  AccumulateData& _accumulateData;
  GreenHouseConfig& _deviceConfig;

  //* Home Assistant Entities
  HADevice device;
  HAMqtt mqtt;

#if USE_DHT_SENSOR
  HASensor tower_humidity;
  HASensor tower_humidity_temp;
#endif  // USE_DHT_SENSOR

  HASensor light;

#if USE_SHT31_SENSOR
  sht31_humidity("tower_humidity_sht31");
  sht31_humidity_temp("tower_humidity_temp_sht31");
  sht31_humidity_2("tower_humidity_sht31");
  sht31_humidity_temp_2("tower_humidity_temp_sht31");
#endif  // USE_SHT31_SENSOR
  HASensor ph;
  HASensor water_level;
  HASensor tower_temp;

  unsigned long lastReadAt;
  unsigned long lastSentAt;
  unsigned long lastInputState;
  unsigned long lastAvailabilityToggleAt;

 public:
  //* Constructor
  HASSMQTT(WiFiClient* espClient,
           PHSENSOR* phsensor,
           BH1750* bh1750,
           LDR* ldr,
           TowerTemp* towertemp,
           Humidity* humidity,
           WaterLevelSensor* waterlevelsensor,
           I2C_RelayBoard* relayboard,
           IPAddress* broker_ip,
           GreenHouseConfig* config);
  virtual ~HASSMQTT();

  void begin();
  void mqttLoop();
  void onMqttMessage(const char* topic,
                     const uint8_t* payload,
                     uint16_t length);
  void onMqttConnected();
  void onMqttConnectionFailed();
  void onBeforeStateChanged(bool state, HASwitch* s);
  void onRelayStateChanged(bool state, HASwitch* s);
};
#endif  // HAMQTT_HPP
