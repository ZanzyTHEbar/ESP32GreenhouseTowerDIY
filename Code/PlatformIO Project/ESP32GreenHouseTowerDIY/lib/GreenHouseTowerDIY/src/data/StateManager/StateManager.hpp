#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

/*
 * StateManager
 * All Project States are managed here
 */
enum _State
{
    Starting,
    ConnectingToWifi,
    ConnectingToWifiError,
    ConnectingToWifiSuccess,
    ServerError,
    PumpError,
    MDNSSuccess,
    MDNSError,
    MQTTSuccess,
    MQTTError,
};

enum _Button_states
{
    UNDEF,
    Buttons_OFF,
    Buttons_ON,
    Buttons_PLUS,
    Buttons_MINUS,
    Buttons_ManAut
};

enum _Pump_state
{
    UNDEF,
    PUMP_OFF,
    PUMP_ON,
    PUMP_MANUAL,
    PUMP_AUTOMATIC,
    PUMP_SERIAL_REPORT
};

/*
 * EventManager
 * All Project Events are managed here
 */

class StateManager
{
public:
    StateManager() : current_state(Starting) {}
    void setState(_State state);
    _State getCurrentState();

private:
    _State current_state;
};
extern StateManager stateManager;
#endif // STATEMANAGER_HPP