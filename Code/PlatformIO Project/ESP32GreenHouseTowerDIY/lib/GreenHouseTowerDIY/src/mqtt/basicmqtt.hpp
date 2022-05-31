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
    int pump_relay_pin;
    char *pumpTopic;
    char *pHTopic;
    char *pHOutTopic;
    int phDnPIN;
    int phUpPIN;
};

extern BASEMQTT basemqtt;
#endif // HAMQTT_HPP