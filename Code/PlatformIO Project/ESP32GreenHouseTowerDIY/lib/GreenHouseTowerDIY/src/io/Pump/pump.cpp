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

PUMP::PUMP() {}

PUMP::~PUMP() {}

void PUMP::begin()
{
    this->pump_data = {
        ._pump_relay_pin = PUMP_RELAY_PIN,
        ._pumpTopic = PUMP_TOPIC,
        ._NOZZLE = PUMP_NOZZLE_PIN,
        ._runInterval = 1,
        ._pumpMaxRunTime = 0,
        ._pumpOn = 0,
        ._nozzleInterval = 10,
        ._nozzleDuration = 1,
        ._runProgram = 1,
        ._oneReport = 0,
        ._t_ = 0,
        ._tDelay = pump_data._runInterval,
    };

    // Setup the pump relay pin
    log_i("Setting up the pump...");
    Relay.RelayOnOff(pump_data._pump_relay_pin, false);
    // Setup the nozzle pin
    pinMode(pump_data._NOZZLE, OUTPUT);
    digitalWrite(pump_data._NOZZLE, LOW);
    // Set the time
    setTime(12, 0, 0, networkntp.getDay().toInt(), networkntp.getMonth().toInt(), networkntp.getYear().toInt()); // hour,min,sec,day,month,year
}

void PUMP::PumpLoop()
{
    pump_data._t_ = now();

    if (pump_data._t_ < pump_data._tDelay)
    {
        return;
    }

    if (pump_data._runProgram == 1)
    {
        scheduleFromUser();

        /**
         * The scheduling is per hour, but running the pump for that long is
         * unnecessary so have a maximum run time that is less than 60 min.
         */
        if (pump_data._pumpOn == 1 && minute(pump_data._t_) > pump_data._pumpMaxRunTime)
        {
            pump_data._pumpOn = 0;
        }
#if USE_BH1750
        setLux(bh1750.getLux());
#else
        setLux(ldr.getLux());
#endif // USE_BH1750
        //setPump();
        //setNozzle();
    }
    if (pump_data._nozzleInterval - 1 == minute(pump_data._t_) % pump_data._nozzleInterval)
    {
        if (pump_data._nozzleDuration > second(pump_data._t_))
        {
            digitalWrite(pump_data._NOZZLE, HIGH);
            pump_data._oneReport = 1;
        }
        if (pump_data._nozzleDuration <= second(pump_data._t_))
        {
            digitalWrite(pump_data._NOZZLE, LOW);
            if (pump_data._oneReport == 1)
            {
                serialReport();
                pump_data._oneReport = 0;
            }
        }
    }

    pump_data._tDelay = pump_data._t_ + pump_data._runInterval;
}

void PUMP::scheduleFromUser()
{
    // Change every hour
    int x = hour(pump_data._t_); // 0-23
    int z = pumpScheduleIndex[0][x];

    pump_data._pumpOn = pumpSchedule[z][0];
    pump_data._nozzleInterval = pumpSchedule[z][1];
    pump_data._nozzleDuration = pumpSchedule[z][2];
}

/**
 * Turn the pump on or off depending on the value of the global variable _pumpOn.
 */
void PUMP::setPump()
{
    Relay.RelayOnOff(pump_data._pump_relay_pin, pump_data._pumpOn);
}

void PUMP::setLux(float lux)
{
    if (lux <= 7)
    {
        pump_data._pumpOn = 0;
    }
    else
    {
        pump_data._pumpOn = 1;
    }
}

void PUMP::setNozzle()
{
    if (pump_data._nozzleDuration == 0)
    {
        digitalWrite(pump_data._NOZZLE, LOW);
    }
}

String PUMP::timeReport()
{
    char buf[100];
    snprintf(buf, sizeof(buf), "Time(dd:hh:mm:ss): %d: %d : %d : %d", day(pump_data._t_), hour(pump_data._t_), minute(pump_data._t_), second(pump_data._t_));
    String timereport = buf;
    return timereport;
}

void PUMP::serialReport()
{
    char buf[1000];
    snprintf(buf, sizeof(buf), "---- Report ----\n Settings:\n Pump: %s \nMinutes between sprays: %d \nSeconds of spray: %d", timeReport().c_str(), pump_data._pumpOn ? "On" : "Off", pump_data._nozzleInterval, pump_data._nozzleDuration);
    String reportValues = buf;
    Serial.println(reportValues);
}

PUMP pump;