#ifndef PUMP_HPP
#define PUMP_HPP
#include <defines.hpp>
#include <TimeLib.h> // For timekeeping#ifndef PUMP_HPP
#include "io/Relays/Relays.hpp"
#include "network/ntp.hpp"
#include "sensors/light/ldr.hpp"

class PUMP
{
private:
    struct Pump_t
    {
        // LED pins (PWM pins: 3,5,6,9,10,11 on arduino UNO)
        byte _pump_relay_pin; // Pin 4 on arduino maps to D2 on wemos
        const char *_pumpTopic;
        int _NOZZLE; // Pin 5 on arduino maps to D1 on wemos
        int _runInterval;
        int _pumpMaxRunTime;

        int _pumpOn;         // 0 off, 1 on
        int _nozzleInterval; // min interval between nozzle activation >60 == off
        int _nozzleDuration; // sec active nozzle <0 == off

        // Run preprogrammed setup, oneReport after nozzle on
        int _runProgram;
        int _oneReport;

        // Time
        time_t _t_;
        time_t _tDelay;
    };

    void scheduleFromUser();
    String timeReport();
    void setLux(float lux);

public:
    PUMP();
    void setPump();
    void setNozzle();
    virtual ~PUMP();
    void begin();
    void PumpLoop();
    void serialReport();

    Pump_t pump_data;

    Pump_t *getPumpData() { return &this->pump_data; };
};
extern PUMP pump;
#endif // PUMP_HPP