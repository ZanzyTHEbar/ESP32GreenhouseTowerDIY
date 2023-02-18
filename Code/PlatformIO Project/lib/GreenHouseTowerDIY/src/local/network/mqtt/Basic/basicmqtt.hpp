/*
 basicmqtt.hpp - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BASEMQTT_HPP
#define BASEMQTT_HPP
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

//*  Sensor Includes
#include <local/io/sensors/humidity/Humidity.hpp>
#include <local/io/sensors/light/bh1750.hpp>
#include <local/io/sensors/light/ldr.hpp>
#include <local/io/sensors/pH/pHsensor.hpp>
#include <local/io/sensors/temperature/towertemp.hpp>
#include <local/io/sensors/water_level/waterlevelsensor.hpp>

/**
 * @brief MQTT Class
 */
class BASEMQTT : public IPAddress, public PubSubClient
{
public:
    // Constructor
    BASEMQTT(WiFiClient *espClient,
             PHSENSOR *phsensor,
             BH1750 *bh1750,
             LDR *ldr,
             TowerTemp *towertemp,
             Humidity *humidity,
             WaterLevelSensor *waterlevelsensor,
             IPAddress *broker_ip);
    virtual ~BASEMQTT();

    void begin();
    void mqttLoop();
    void mqttReconnect();
    void mqttCallback(char *topic, byte *payload, unsigned int length);

    // Friends
    friend class LDR;

private:
    PHSENSOR *phsensor;
    BH1750 *bh1750;
    LDR *ldr;
    TowerTemp *towertemp;
    Humidity *humidity;
    WaterLevelSensor *waterlevelsensor;

    // Private variables
    const long _interval;
    unsigned long _previousMillis;
    uint8_t _user_bytes_received;
    char _user_data[100];
};
#endif // HAMQTT_HPP
