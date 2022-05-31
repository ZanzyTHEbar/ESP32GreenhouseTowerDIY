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
    char *_pHTopic;
    char *_pHOutTopic;
    int _phDnPIN;
    int _phUpPIN;
    int _doseTimeSm;
    int _doseTimeMed;
    int _doseTimeLg;
};

extern PHSENSOR phsensor;
#endif // PHSENSOR_HPP