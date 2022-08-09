#pragma once
#ifndef BASEMQTT_HPP
#define BASEMQTT_HPP
#include <defines.hpp>
#include "network/network.hpp"
#include "network/ntp.hpp"
#include "io/Pump/pump.hpp"
#include "io/Relays/Relays.hpp"
#include "sensors/pH/pHsensor.hpp"
#include "data/StateManager/StateManager.hpp"
#include "data/AccumulateData/accumulatedata.hpp"

class BaseMQTT : public Network
{
public:
    // Constructor
    BaseMQTT();
    virtual ~BaseMQTT();

    virtual bool begin();

    template <typename T>
    void checkState(T mqttClient)
    {
        cfg.config.MQTTConnectedState = mqttClient.connected();

        cfg.config.MQTTConnectedState ? StateManager_MQTT.setState(ProgramStates::DeviceStates::MQTTState_e::MQTT_Connected) : StateManager_MQTT.setState(ProgramStates::DeviceStates::MQTTState_e::MQTT_Disconnected);
        log_i("MQTT client state is: %d", mqttClient.state());
    }

    void loadMQTTConfig();
    void mqttLoop();

    // Handles messages arrived on subscribed topic(s)

    void callback(char *topic, byte *payload, unsigned int length)
    {
        std::string result = "";
        log_i("Message arrived on topic: [%s] ", topic);
        char buffer[length + 1];
        memcpy(buffer, payload, length);
        buffer[length] = '\0'; // Null-terminate the string
        log_i("payload is: [%s]", buffer);
        result += buffer;
        log_i("Message: [%s]", result.c_str());

        // Check if the message is for the current topic
        switch (s_callback_map[topic])
        {
        case Info:
            break;
        case Menu:
            break;
        case Relays:
            break;
#if ENABLE_PH_SUPPORT
        case Ph:
            log_i("Setting pH level to: [%s]", buffer);
            phsensor.parse_cmd_lookup(result);
            break;
#endif // ENABLE_PH_SUPPORT
        case Pump:
            switch (s_relay_control_map[result])
            {
            case On:
                log_i("Turning on the pump");
                Relay.RelayOnOff(pump.pump_data._pump_relay_pin, true);
                break;
            case Off:
                log_i("Turning off the pump");
                Relay.RelayOnOff(pump.pump_data._pump_relay_pin, false);
                break;
            case UnDefined:
                log_e("Undefined command");
                break;
            case End:
                //! Implement a method to turn off MQTT
                break;
            }
            break;
        default:
            break;
        }
    }

    void callback(const char *topic, const uint8_t *payload, uint16_t length)
    {
        std::string result = "";
        log_i("Message arrived on topic: [%s] ", topic);
        char buffer[length + 1];
        memcpy(buffer, payload, length);
        buffer[length] = '\0'; // Null-terminate the string
        result += buffer;
        log_i("payload is: [%s]", buffer);
        result += buffer;
        log_i("Message: [%s]", result.c_str());

        // Check if the message is for the current topic
        switch (s_callback_map[topic])
        {
        case Info:
            break;
        case Menu:
            break;
        case Relays:
            break;
#if ENABLE_PH_SUPPORT
        case Ph:
            log_i("Setting pH level to: [%s]", result.c_str());
            phsensor.parse_cmd_lookup(result);
            break;
#endif // ENABLE_PH_SUPPORT
        case Pump:
            switch (s_relay_control_map[result])
            {
            case On:
                log_i("Turning on the pump");
                Relay.RelayOnOff(pump.pump_data._pump_relay_pin, true);
                break;
            case Off:
                log_i("Turning off the pump");
                Relay.RelayOnOff(pump.pump_data._pump_relay_pin, false);
                break;
            case UnDefined:
                log_e("Undefined command");
                break;
            case End:
                //! Implement a method to turn off MQTT
                break;
            }
            break;
        default:
            break;
        }
    }
    // Get's the broker address
    const char *getBrokerAddress();

private:
    // Private functions

    // Private variables
    unsigned long lastReadAt;
    unsigned long lastAvailabilityToggleAt;
    bool lastInputState;
    unsigned long lastSentAt;

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
        Relays,
        Ph,
        Info,
        Menu,
        EndCallback
    };

    static std::unordered_map<std::string, RelayEnum> s_relay_control_map;
    static std::unordered_map<std::string, PumpEnum> s_pump_control_map;
    static std::unordered_map<std::string, CallbackEnum> s_callback_map;
};
#endif // BASEMQTT_HPP