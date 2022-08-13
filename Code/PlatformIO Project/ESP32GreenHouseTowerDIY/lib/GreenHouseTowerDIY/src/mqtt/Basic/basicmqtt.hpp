/*
 basicMqtt.hpp - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BASICMQTT_HPP
#define BASICMQTT_HPP
#include <PubSubClient.h>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <memory>
#include "mqtt/mqttbase.hpp"

class BasicMqtt
{
public:
    // Constructor
    BasicMqtt(Network *network,
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
    virtual ~BasicMqtt();

    bool begin();
    void mqttLoop();
    void checkState();

    void mqttReconnect();

private:
    // Private functions

    // Private variables
    const long _interval;
    const long _interval_reconnect;
    unsigned long _previousMillis;
    uint8_t _user_bytes_received;
    char _user_data[100];

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

extern BasicMqtt basicmqtt;
#endif // HAMQTT_HPP