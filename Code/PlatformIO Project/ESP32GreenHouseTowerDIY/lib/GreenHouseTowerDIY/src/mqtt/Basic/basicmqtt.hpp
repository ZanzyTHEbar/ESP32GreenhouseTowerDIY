/*
 basicmqtt.hpp - ESP32GreenHouseDIY MQTT library
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
#include "mqtt/base.hpp"

class BASICMQTT : public BaseMQTT, IPAddress
{
public:
    // Constructor
    BASICMQTT();
    virtual ~BASICMQTT();

    bool begin();
    void loadMQTTConfig();
    void checkState();
    void mqttReconnect();
    void mqttLoop();

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

extern BASICMQTT basicmqtt;
#endif // HAMQTT_HPP