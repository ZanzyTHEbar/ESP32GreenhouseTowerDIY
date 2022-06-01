/*
 basicmqtt.hpp - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BASEMQTT_HPP
#define BASEMQTT_HPP
#include <defines.hpp>
#include <PubSubClient.h>

class BASEMQTT
{
public:
    // Constructor
    BASEMQTT();
    virtual ~BASEMQTT();

    void loadMQTTConfig();
    void mqttSetup();
    void mqttLoop();
    void mqttReconnect();

    // Friends
    friend class LDR;
    friend void callback(char *topic, byte *payload, unsigned int length);

private:
    // Private functions

    // Private variables
    const long _interval;
    unsigned long _previousMillis;
    uint8_t _user_bytes_received;
    char _user_data[100];
};

extern BASEMQTT basemqtt;
#endif // HAMQTT_HPP