/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef PHSENSOR_HPP
#define PHSENSOR_HPP
#include <defines.hpp>
#include <ph_grav.h>

class PHSENSOR
{
public:
    // Constructor
    PHSENSOR();
    virtual ~PHSENSOR();

    void phSensorSetup();
    void eventListener(const char *topic, const uint8_t *payload, uint16_t length);
    void parse_cmd(char *string);
    void getPH();

private:
    // Private functions

    // Private variables
    char *pHTopic;
    char *pHOutTopic;
    int phDnPIN;
    int phUpPIN;
    int doseTimeSm;
    int doseTimeMed;
    int doseTimeLg;
};

extern PHSENSOR phsensor;
#endif // PHSENSOR_HPP