#include "pump.hpp"

/**
 * Schedule of all settings
 */
int pumpSchedule[3][3] = {
    {0, 10, 1}, // Spray the roots at a set interval and duration (Default)
    {1, 10, 1}, // Run the pump (Default)
    {0, 10, 1}  // Night setting (Default)
};
// Day setting set (0:00-23:00)
int pumpScheduleIndex[1][24] = {
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2} // Pump at 12:00
};

PUMP::PUMP()
{
    pump_relay_pin = PUMP_RELAY_PIN;
    NOZZLE = PUMP_NOZZLE_PIN;
    runInterval = 1;
    pumpMaxRunTime = 0;

    pumpOn = 0;          // 0 off, 1 on
    nozzleInterval = 10; // min interval between nozzle activation >60 == off
    nozzleDuration = 1;  // sec active nozzle <0 == off

    // Run preprogrammed setup, oneReport after nozzle on
    runProgram = 1;
    oneReport = 0;

    // Time
    t = 0;
    tDelay = runInterval;
}

PUMP::~PUMP()
{
}

void PUMP::SetupPump()
{
    log_i("Setting up the pump...");
    Relay.RelayOnOff(pump_relay_pin, false);

    pinMode(NOZZLE, OUTPUT);
    digitalWrite(NOZZLE, LOW);

    pinMode(LED_BUILTIN, OUTPUT); // Pin 13 on arduino and 2 on wemos
    digitalWrite(LED_BUILTIN, LOW);

    // Set the time
    setTime(12, 0, 0, networkntp.getDay().toInt(), networkntp.getMonth().toInt(), networkntp.getYear().toInt()); // hour,min,sec,day,month,year
}

PUMP pump;