/*
 basicmqtt.hpp - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BASEMQTT_HPP
#define BASEMQTT_HPP
#include <defines.hpp>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

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
    char *pHTopic;
    char *pHOutTopic;
    int phDnPIN;
    int phUpPIN;

    // Variables to save date and time
    String formattedDate;
    String dayStamp;
    String timeStamp;
};

extern BASEMQTT basemqtt;
#endif // HAMQTT_HPP