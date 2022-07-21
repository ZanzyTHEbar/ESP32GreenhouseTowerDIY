#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP
#include <Arduino.h>

/*
 * StateManager
 * All Project States are managed here
 */
class ProgramStates
{
public:
    struct DeviceStates
    {
        enum State_e
        {
            Starting,
            Started,
            Stopping,
            Stopped,
            Error

        };

        enum WiFiState_e
        {
            WiFiState_None,
            WiFiState_Connecting,
            WiFiState_Connected,
            WiFiState_Disconnected,
            WiFiState_Disconnecting,
            WiFiState_Error
        };

        enum WebServerState_e
        {
            WebServerState_None,
            WebServerState_Starting,
            WebServerState_Started,
            WebServerState_Stopping,
            WebServerState_Stopped,
            WebServerState_Error
        };

        enum MDNSState_e
        {
            MDNS_Success,
            MDNS_Connected,
            MDNS_Disconnected,
            MDNS_Error
        };

        enum MQTTState_e
        {
            MQTT_Success,
            MQTT_Connected,
            MQTT_Disconnected,
            MQTT_Error
        };

        enum ButtonState_e
        {
            Buttons_OFF,
            Buttons_ON,
            Buttons_PLUS,
            Buttons_MINUS,
            Buttons_ManAut,
            Buttons_Error
        };

        enum PumpState_e
        {
            PUMP_OFF,
            PUMP_ON,
            PUMP_MANUAL,
            PUMP_AUTOMATIC,
            PUMP_SERIAL_REPORT,
            PUMP_Error
        };
    };
};

/*
 * EventManager
 * All Project Events are managed here
 */
template <class T>
class StateManager
{
public:
    StateManager() {}

    virtual ~StateManager() {}

    void setState(T state)
    {
        _current_state = state;
    }

    /*
     * Get States
     * Returns the current state of the device
     */
    T getCurrentState()
    {
        return _current_state;
    }

private:
    T _current_state;
};

extern StateManager<ProgramStates::DeviceStates::State_e> StateManager_Device;
extern StateManager<ProgramStates::DeviceStates::WiFiState_e> StateManager_WiFi;
extern StateManager<ProgramStates::DeviceStates::WebServerState_e> StateManager_WebServer;
extern StateManager<ProgramStates::DeviceStates::MDNSState_e> StateManager_MDNS;
extern StateManager<ProgramStates::DeviceStates::MQTTState_e> StateManager_MQTT;
extern StateManager<ProgramStates::DeviceStates::ButtonState_e> StateManager_Buttons;
extern StateManager<ProgramStates::DeviceStates::PumpState_e> StateManager_Pump;

#endif // STATEMANAGER_HPP