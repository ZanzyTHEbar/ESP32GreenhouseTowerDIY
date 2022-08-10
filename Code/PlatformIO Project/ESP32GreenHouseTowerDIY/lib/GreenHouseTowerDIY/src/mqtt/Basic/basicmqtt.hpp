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
    BasicMqtt();
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

    const PHSENSOR::ph_Data_t &phData = phsensor.ph_data.at("id");
};

extern BasicMqtt basicmqtt;
#endif // HAMQTT_HPP