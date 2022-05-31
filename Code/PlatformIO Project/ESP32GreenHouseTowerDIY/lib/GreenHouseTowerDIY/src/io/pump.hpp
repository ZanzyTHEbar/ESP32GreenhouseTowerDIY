#ifndef PUMP_HPP
#define PUMP_HPP
#include <defines.hpp>
#include <TimeLib.h> // For timekeeping#ifndef PUMP_HPP

class PUMP
{
public:
    PUMP();
    virtual ~PUMP();
    void SetupPump();
    void PumpLoop();
    void serialControl();
    void scheduleFromUser();
    void setPump();
    void setNozzle();
    void serialReport();
    String timeReport();


private:
    // LED pins (PWM pins: 3,5,6,9,10,11 on arduino UNO)
    int pump_relay_pin = PUMP_RELAY_PIN; // Pin 4 on arduino maps to D2 on wemos
    int NOZZLE;                          // Pin 5 on arduino maps to D1 on wemos
    int runInterval;
    int pumpMaxRunTime;

    int pumpOn;         // 0 off, 1 on
    int nozzleInterval; // min interval between nozzle activation >60 == off
    int nozzleDuration; // sec active nozzle <0 == off

    // Run preprogrammed setup, oneReport after nozzle on
    int runProgram;
    int oneReport;

    // Time
    time_t t;
    time_t tDelay;
};
extern PUMP pump;
#endif // PUMP_HPP