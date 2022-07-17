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

    void phSensorLoop();
    void eventListener(const char *topic, const uint8_t *payload, uint16_t length);
    void parse_cmd(char *string);
    float getPH();

    // Friends
    friend class BASEMQTT;
    friend void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length);
    friend void callback(char *topic, byte *payload, unsigned int length);
    friend void onMqttConnected();
    friend class HASSMQTT;

private:
    // Private functions
    void serialEvent();

    // Private variables
    const char *_pHTopic;
    const char *_pHOutTopic;
    int _phDnPIN;
    int _phUpPIN;
    int _doseTimeSm;
    int _doseTimeMed;
    int _doseTimeLg;
    String _inputstring;            // a string to hold incoming data from the PC
    boolean _input_string_complete; // a flag to indicate have we received all the data from the PC
    char _inputstring_array[10];    // a char array needed for string parsing
};

extern PHSENSOR phsensor;
#endif // PHSENSOR_HPP