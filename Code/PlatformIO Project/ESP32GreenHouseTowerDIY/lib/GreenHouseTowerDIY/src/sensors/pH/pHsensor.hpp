/*
 HMSMqtt.h - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef PHSENSOR_HPP
#define PHSENSOR_HPP
#include <defines.hpp>
#include <ph_grav.h>
#include <vector>
#include <memory>
#include <map>

class PHSENSOR
{
public:
    // Constructor
    PHSENSOR();
    virtual ~PHSENSOR();

    void begin();

    void phSensorLoop();
    void eventListener(const char *topic, const uint8_t *payload, uint16_t length);
    float getPH();

    // Friends
    friend class BASEMQTT;
    friend void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length);
    friend void callback(char *topic, byte *payload, unsigned int length);
    friend void onMqttConnected();
    friend class HASSMQTT;

    typedef void (Gravity_pH::*map_method_t)(void);
    typedef std::map<std::string, map_method_t> _pHcommandMap;

    typedef void (PHSENSOR::*map_custom_method_t)(void);
    typedef std::map<std::string, map_custom_method_t> _phmap;

private:
    // Private functions
    // void parse_cmd(const char *string);
    void parse_cmd_lookup(std::string index);
    void setPHUpPin();
    void setPHDnPin();
    void serialEvent();

    // Private variables
    int _phDnPIN;
    int _phUpPIN;
    int _doseTimeSm;
    int _doseTimeMed;
    int _doseTimeLg;
    const char *_pHTopic;
    const char *_pHOutTopic;
    String _inputstring;            // a string to hold incoming data from the PC
    boolean _input_string_complete; // a flag to indicate have we received all the data from the PC
    char _inputstring_array[10];    // a char array needed for string parsing
    std::shared_ptr<Gravity_pH> _pH;
};
extern PHSENSOR phsensor;
#endif // PHSENSOR_HPP