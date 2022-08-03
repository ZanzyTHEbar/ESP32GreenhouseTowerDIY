#ifndef RELAYS_HPP
#define RELAYS_HPP
#include <defines.hpp>
#include <PID_v1.h>

class Relays
{
public:
    Relays();
    virtual ~Relays();
    bool begin();
    void RelayOnOff(int relay, bool state, long double delay = 0.1L);
    void SetupPID();
#if USE_SHT31_SENSOR
    void HumRelayOnOff();
#endif // USE_SHT31_SENSOR

private:

#if USE_PID
    double _Setpoint, _Input, _Output;
    int _WindowSize;
    unsigned long _windowStartTime;
    PID myPID;
#endif // USE_PID
};
extern Relays Relay;
#endif