#ifndef PUMP_HPP
#define PUMP_HPP
#include <defines.hpp>
#include <TimeLib.h> // For timekeeping#ifndef PUMP_HPP

class PUMP
{
public:
    PUMP();
    virtual ~PUMP();
    void PumpLoop();
    void serialControl();
    void scheduleFromUser();
    void setPump();
    void setLux(float lux);
    void setNozzle();
    void serialReport();
    String timeReport();

    // Friends
    friend class LDR;
    friend class BASEMQTT;
    friend void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length);
    friend void callback(char *topic, byte *payload, unsigned int length);
    friend void onMqttConnected();
    friend class HASSMQTT;

private:
    // LED pins (PWM pins: 3,5,6,9,10,11 on arduino UNO)
    int _pump_relay_pin; // Pin 4 on arduino maps to D2 on wemos
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
extern PUMP pump;
#endif // PUMP_HPP