#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

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

class StateManager
{
public:
    StateManager();
    virtual ~StateManager();

    void setState(ProgramStates::DeviceStates::State_e state);
    void setState(ProgramStates::DeviceStates::WiFiState_e state);
    void setState(ProgramStates::DeviceStates::WebServerState_e state);
    void setState(ProgramStates::DeviceStates::MDNSState_e state);
    void setState(ProgramStates::DeviceStates::MQTTState_e state);
    void setState(ProgramStates::DeviceStates::ButtonState_e state);
    void setState(ProgramStates::DeviceStates::PumpState_e state);

    ProgramStates::DeviceStates::State_e getCurrentState();
    ProgramStates::DeviceStates::WiFiState_e getCurrentWiFiState();
    ProgramStates::DeviceStates::WebServerState_e getCurrentWebServerState();
    ProgramStates::DeviceStates::MDNSState_e getCurrentMDNSState();
    ProgramStates::DeviceStates::MQTTState_e getCurrentMQTTState();
    ProgramStates::DeviceStates::ButtonState_e getCurrentButtonState();
    ProgramStates::DeviceStates::PumpState_e getCurrentPumpState();

private:
    ProgramStates::DeviceStates::State_e _current_state;
    ProgramStates::DeviceStates::WiFiState_e _current_wifi_state;
    ProgramStates::DeviceStates::WebServerState_e _current_webserver_state;
    ProgramStates::DeviceStates::MDNSState_e _current_mdns_state;
    ProgramStates::DeviceStates::MQTTState_e _current_mqtt_state;
    ProgramStates::DeviceStates::ButtonState_e _current_button_state;
    ProgramStates::DeviceStates::PumpState_e _current_pump_state;
};
extern StateManager stateManager;
#endif // STATEMANAGER_HPP