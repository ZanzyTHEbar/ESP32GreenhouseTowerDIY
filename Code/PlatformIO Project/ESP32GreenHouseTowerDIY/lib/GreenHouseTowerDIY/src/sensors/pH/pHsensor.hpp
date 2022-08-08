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
#include <functional>

class PHSENSOR
{
private:

    // Private functions
    // void parse_cmd(const char *string);
    void setPHUpPin();
    void setPHDnPin();

    unsigned long PreviousMillis = 0;
    std::shared_ptr<Gravity_pH> _pH;

public:
    // Constructor
    PHSENSOR();
    virtual ~PHSENSOR();

    void begin();
    float getPH();
    void parse_cmd_lookup(std::string index);

    // void phSensorLoop();
    // void serialEvent();

    // Friends
    friend class BASEMQTT;
    friend void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length);
    friend void callback(char *topic, byte *payload, unsigned int length);
    friend void onMqttConnected();
    friend class HASSMQTT;

    typedef void (Gravity_pH::*map_method_t)(void);
    typedef std::map<std::string, map_method_t> _pHcommandMap;

    typedef std::function<void(uint8_t *, int *)> func;
    std::map<std::string, func> phmap;

    void custom_delay(int time)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - PreviousMillis >= time)
        {
            PreviousMillis = currentMillis;
            return;
        }
    }

    void setPHPin(uint8_t *pin, int *time, bool state);

    struct ph_Data_t
    {
        // Private variables
        uint8_t _phDnPIN;
        uint8_t _phUpPIN;
        int _doseTimeSm;
        int _doseTimeMed;
        int _doseTimeLg;
        const char *_pHTopic;
        const char *_pHOutTopic;
        String _inputstring;            // a string to hold incoming data from the PC
        boolean _input_string_complete; // a flag to indicate have we received all the data from the PC
        char _inputstring_array[10];    // a char array needed for string parsing
    };

    ph_Data_t _phData;
    ph_Data_t *getphData(void) { return &this->_phData; }
};
extern PHSENSOR phsensor;
#endif // PHSENSOR_HPP