/*
 basicmqtt.hpp - ESP32GreenHouseDIY MQTT library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BASEMQTT_HPP
#define BASEMQTT_HPP
#include <defines.hpp>
#include <PubSubClient.h>
#include <map>
#include <string>
#include <iostream>

#include "network/network.hpp"
#include "network/ntp.hpp"
#include "io/Pump/pump.hpp"
#include "io/Relays/Relays.hpp"
#include "sensors/pH/pHsensor.hpp"
#include "data/StateManager/StateManager.hpp"

class BASEMQTT : public Network, IPAddress
{
public:
    // Constructor
    BASEMQTT();
    virtual ~BASEMQTT();

    bool begin();
    void loadMQTTConfig();
    void checkState();
    void mqttReconnect();
    void mqttLoop();

    // Friends
    friend class LDR;
    friend void callback(char *topic, byte *payload, unsigned int length);

private:
    // Private functions

    // Private variables
    const long _interval;
    const long _interval_reconnect;
    unsigned long _previousMillis;
    uint8_t _user_bytes_received;
    char _user_data[100];

    const char *_infoTopic;
    const char *_statusTopic;
    const char *_commandTopic;
    const char *_configTopic;
    const char *_menuTopic;

    // Value-Defintions of the different String values
    enum RelayEnum
    {
        UnDefined,
        On,
        Off,
        End
    };

    enum PumpEnum
    {
        UnDefinedPump,
        PumpOn,
        PumpOff,
        PumpEnd
    };

    enum CallbackEnum
    {
        UnDefinedCallback,
        Pump,
        Relay,
        Ph,
        Info,
        Menu,
        EndCallback
    };

    static std::map<std::string, RelayEnum> s_relay_control_map;
    static std::map<std::string, PumpEnum> s_pump_control_map;
    static std::map<std::string, CallbackEnum> s_callback_map;
};

extern BASEMQTT basemqtt;
#endif // HAMQTT_HPP