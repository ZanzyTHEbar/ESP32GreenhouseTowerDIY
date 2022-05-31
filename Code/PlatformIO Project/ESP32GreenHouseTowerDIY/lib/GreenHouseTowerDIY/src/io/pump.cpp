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
    _pump_relay_pin = PUMP_RELAY_PIN;
    _pumpTopic = PUMP_TOPIC;
    _NOZZLE = PUMP_NOZZLE_PIN;
    _runInterval = 1;
    _pumpMaxRunTime = 0;

    _pumpOn = 0;          // 0 off, 1 on
    _nozzleInterval = 10; // min interval between _nozzle activation >60 == off
    _nozzleDuration = 1;  // sec active _nozzle <0 == off

    // Run preprogrammed setup, _oneReport after _nozzle on
    _runProgram = 1;
    _oneReport = 0;

    // Time
    _t_ = 0;
    _tDelay = _runInterval;
}

PUMP::~PUMP()
{
}

void PUMP::SetupPump()
{
    log_i("Setting up the pump...");
    Relay.RelayOnOff(_pump_relay_pin, false);

    pinMode(_NOZZLE, OUTPUT);
    digitalWrite(_NOZZLE, LOW);

    pinMode(LED_BUILTIN, OUTPUT); // Pin 13 on arduino and 2 on wemos
    digitalWrite(LED_BUILTIN, LOW);

    // Set the time
    setTime(12, 0, 0, networkntp.getDay().toInt(), networkntp.getMonth().toInt(), networkntp.getYear().toInt()); // hour,min,sec,day,month,year
}

void PUMP::PumpLoop()
{
    _t_ = now();
    // Keep working as long as data is in the buffer
    while (Serial.available() > 0)
    {
        serialControl();
    }
    if (_t_ < _tDelay)
    {
        return;
    }

    if (_runProgram == 1)
    {
        scheduleFromUser();

        /**
         * The scheduling is per hour, but running the pump for that long is
         * unnecessary so have a maximum run time that is less than 60 min.
         */
        if (_pumpOn == 1 && minute(_t_) > _pumpMaxRunTime)
        {
            _pumpOn = 0;
        }
        setPump();
    }
    if (_nozzleInterval - 1 == minute(_t_) % _nozzleInterval)
    {
        if (_nozzleDuration > second(_t_))
        {
            digitalWrite(_NOZZLE, HIGH);
            _oneReport = 1;
        }
        if (_nozzleDuration <= second(_t_))
        {
            digitalWrite(_NOZZLE, LOW);
            if (_oneReport == 1)
            {
                _oneReport = 0;
                serialReport();
            }
        }
    }

    _tDelay = _t_ + _runInterval;
}

void PUMP::scheduleFromUser()
{
    // Change every hour
    int x = hour(_t_); // 0-23
    int z = pumpScheduleIndex[0][x];

    _pumpOn = pumpSchedule[z][0];
    _nozzleInterval = pumpSchedule[z][1];
    _nozzleDuration = pumpSchedule[z][2];
}

/**
 * Turn the pump on or off depending on the value of the global variable _pumpOn.
 */
void PUMP::setPump()
{
    if (_pumpOn == 1)
    {
        Relay.RelayOnOff(_pump_relay_pin, true);
    }
    if (_pumpOn == 0)
    {
        Relay.RelayOnOff(_pump_relay_pin, false);
    }
}

void PUMP::setNozzle()
{
    if (_nozzleDuration == 0)
    {
        digitalWrite(_NOZZLE, LOW);
    }
}

String PUMP::timeReport()
{
    String timeString;

    timeString = "Time(dd:hh:mm:ss):      ";
    timeString = timeString + day(_t_);
    timeString = timeString + ":";
    timeString = timeString + hour(_t_);
    timeString = timeString + ":";
    timeString = timeString + minute(_t_);
    timeString = timeString + ":";
    timeString = timeString + second(_t_);
    timeString = timeString + "\n";

    return timeString;
}

void PUMP::serialReport()
{
    String reportValues;

    reportValues = "Settings:\n";
    reportValues = "Pump(0=off, 1=on):      ";
    reportValues = reportValues + _pumpOn;
    reportValues = reportValues + "\nMinutes between sprays: ";
    reportValues = reportValues + _nozzleInterval;
    reportValues = reportValues + "\nSeconds of spray:       ";
    reportValues = reportValues + _nozzleDuration;

    reportValues = "-- Report ----\n" + timeReport() + reportValues;
    Serial.println(reportValues);
}

/*
 * Messages consist of the letter S followed by
 * ,int(0-1),int(0-59),int(0-59)
 * S,0,20,5 (pump,_nozzleInterval,_nozzleDuration)
 */
void PUMP::serialControl()
{
    char c = Serial.read();
    switch (c)
    {
    case 'S':
    {
        _pumpOn = Serial.parseInt();
        _nozzleInterval = Serial.parseInt();
        _nozzleDuration = Serial.parseInt();

        setPump();
        setNozzle();
        break;
    }
    case 'R':
    {
        serialReport();
        break;
    }
    case 'Q':
    {
        if (_runProgram == 1)
        {
            _runProgram = 0;
            break;
        }
        if (_runProgram == 0)
        {
            _runProgram = 1;
            break;
        }
        break;
    }
    case 'T':
    {
        int h = Serial.parseInt();    // First valid integer
        int m = Serial.parseInt();    // Second valid integer
        int d = Serial.parseInt();    // Third valid integer
        setTime(h, m, 0, d, 1, 2014); // hour,min,sec,day,month,year
        _t_ = now();
        _tDelay = _runInterval;
        break;
    }
    }
}

PUMP pump;