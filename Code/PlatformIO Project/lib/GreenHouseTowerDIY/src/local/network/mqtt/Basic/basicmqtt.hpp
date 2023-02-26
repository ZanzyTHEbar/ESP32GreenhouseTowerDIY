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

//*  Sensor Includes
#include <local/io/sensors/humidity/Humidity.hpp>
#include <local/io/sensors/light/bh1750.hpp>
#include <local/io/sensors/light/ldr.hpp>
#include <local/io/sensors/pH/pHsensor.hpp>
#include <local/io/sensors/temperature/towertemp.hpp>
#include <local/io/sensors/water_level/waterlevelsensor.hpp>

//* IO Includes
#include <local/io/Relays/relays.hpp>

/**
 * @brief MQTT Class
 */
class BASEMQTT : public IPAddress, public PubSubClient {
  PHSENSOR* phsensor;
  BH1750* bh1750;
  LDR* ldr;
  TowerTemp* towertemp;
  Humidity* humidity;
  WaterLevelSensor* waterlevelsensor;
  I2C_RelayBoard* relayboard;
  GreenHouseConfig* deviceConfig;

  // Private variables
  const long _interval;
  unsigned long _previousMillis;

 public:
  // Constructor
  BASEMQTT(WiFiClient* espClient,
           PHSENSOR* phsensor,
           BH1750* bh1750,
           LDR* ldr,
           TowerTemp* towertemp,
           Humidity* humidity,
           WaterLevelSensor* waterlevelsensor,
           I2C_RelayBoard* relayboard,
           IPAddress* broker_ip,
           GreenHouseConfig* config);
  virtual ~BASEMQTT();

  void begin();
  void mqttLoop();
  void mqttReconnect();
  void subscribeAll();
  void publishAll();
  void mqttCallback(char* topic, byte* payload, unsigned int length);
};
#endif  // HAMQTT_HPP
