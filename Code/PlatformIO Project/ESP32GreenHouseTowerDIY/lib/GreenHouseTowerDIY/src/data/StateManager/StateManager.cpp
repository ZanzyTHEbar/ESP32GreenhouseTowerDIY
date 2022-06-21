#include "StateManager.hpp"

StateManager::StateManager() : _current_state(ProgramStates::DeviceStates::State_e::Starting),
                               _current_wifi_state(ProgramStates::DeviceStates::WiFiState_e::WiFiState_None),
                               _current_webserver_state(ProgramStates::DeviceStates::WebServerState_e::WebServerState_None),
                               _current_mdns_state(ProgramStates::DeviceStates::MDNSState_e::MDNS_Disconnected),
                               _current_mqtt_state(ProgramStates::DeviceStates::MQTTState_e::MQTT_Disconnected),
                               _current_button_state(ProgramStates::DeviceStates::ButtonState_e::Buttons_OFF),
                               _current_pump_state(ProgramStates::DeviceStates::PumpState_e::PUMP_OFF) {}

StateManager::~StateManager() {}

void StateManager::setState(ProgramStates::DeviceStates::State_e state)
{
    _current_state = state;
}

void StateManager::setState(ProgramStates::DeviceStates::WiFiState_e state)
{
    _current_wifi_state = state;
}

void StateManager::setState(ProgramStates::DeviceStates::WebServerState_e state)
{
    _current_webserver_state = state;
}

void StateManager::setState(ProgramStates::DeviceStates::MDNSState_e state)
{
    _current_mdns_state = state;
}

void StateManager::setState(ProgramStates::DeviceStates::MQTTState_e state)
{
    _current_mqtt_state = state;
}

void StateManager::setState(ProgramStates::DeviceStates::ButtonState_e state)
{
    _current_button_state = state;
}

void StateManager::setState(ProgramStates::DeviceStates::PumpState_e state)
{
    _current_pump_state = state;
}

/*
 * Get States
 * Returns the current state of the device
 */

ProgramStates::DeviceStates::State_e StateManager::getCurrentState()
{
    return _current_state;
}

ProgramStates::DeviceStates::WiFiState_e StateManager::getCurrentWiFiState()
{
    return _current_wifi_state;
}

ProgramStates::DeviceStates::WebServerState_e StateManager::getCurrentWebServerState()
{
    return _current_webserver_state;
}

ProgramStates::DeviceStates::MDNSState_e StateManager::getCurrentMDNSState()
{
    return _current_mdns_state;
}

ProgramStates::DeviceStates::MQTTState_e StateManager::getCurrentMQTTState()
{
    return _current_mqtt_state;
}

ProgramStates::DeviceStates::ButtonState_e StateManager::getCurrentButtonState()
{
    return _current_button_state;
}

ProgramStates::DeviceStates::PumpState_e StateManager::getCurrentPumpState()
{
    return _current_pump_state;
}

StateManager stateManager;