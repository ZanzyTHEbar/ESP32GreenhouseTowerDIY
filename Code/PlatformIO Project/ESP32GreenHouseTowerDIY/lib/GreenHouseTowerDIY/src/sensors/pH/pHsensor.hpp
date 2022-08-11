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
#include <unordered_map>
#include <string>
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
    void parse_cmd_lookup(std::string &index);

    template <typename T>
    void addCommand(std::string command, T function)
    {
        phmap.emplace(command, function);
    }

    // void phSensorLoop();
    // void serialEvent();

    using MapMethod_t = void (Gravity_pH::*)(void);
    using PhCommandMap_t = std::unordered_map<std::string, MapMethod_t>;

    PhCommandMap_t mapping;

    typedef std::function<void (uint8_t *, int *)> Func_t;
    typedef std::unordered_map<std::string, Func_t> PhMap_t;
    PhMap_t phmap;

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
        uint8_t id;
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

    std::unordered_map<std::string, ph_Data_t> ph_data_map;

    const std::unordered_map<std::string, ph_Data_t> &ph_data = ph_data_map; // a read-only reference to the map which is used to get the data without mutation

    bool operator==(const PHSENSOR &other) const
    {
        // this->_phData.id == other._phData.id && this->_phData._phDnPIN == other._phData._phDnPIN && this->_phData._phUpPIN == other._phData._phUpPIN && this->_phData._doseTimeSm == other._phData._doseTimeSm && this->_phData._doseTimeMed == other._phData._doseTimeMed && this->_phData._doseTimeLg == other._phData._doseTimeLg && this->_phData._pHTopic == other._phData._pHTopic && this->_phData._pHOutTopic == other._phData._pHOutTopic && this->_phData._inputstring == other._phData._inputstring && this->_phData._input_string_complete == other._phData._input_string_complete && this->_phData._inputstring_array == other._phData._inputstring_array
        return *this == other;
    }

    bool operator!=(const PHSENSOR &other) const
    {
        return !(*this == other);
    }
};

//! IMPORTANT: implement later
namespace std
{
    template <>
    struct hash<PHSENSOR>
    {
        size_t operator()(const PHSENSOR &key) const
        {
            return 0;
        }
    };
}
#endif // PHSENSOR_HPP