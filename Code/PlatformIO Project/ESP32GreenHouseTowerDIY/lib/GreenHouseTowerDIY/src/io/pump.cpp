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

void PUMP::PumpLoop()
{
    t = now();
    // Keep working as long as data is in the buffer
    while (Serial.available() > 0)
    {
        serialControl();
    }
    if (t < tDelay)
    {
        return;
    }

    if (runProgram == 1)
    {
        scheduleFromUser();

        /**
         * The scheduling is per hour, but running the pump for that long is
         * unnecessary so have a maximum run time that is less than 60 min.
         */
        if (pumpOn == 1 && minute(t) > pumpMaxRunTime)
        {
            pumpOn = 0;
        }
        setPump();
    }
    if (nozzleInterval - 1 == minute(t) % nozzleInterval)
    {
        if (nozzleDuration > second(t))
        {
            digitalWrite(NOZZLE, HIGH);
            oneReport = 1;
        }
        if (nozzleDuration <= second(t))
        {
            digitalWrite(NOZZLE, LOW);
            if (oneReport == 1)
            {
                oneReport = 0;
                serialReport();
            }
        }
    }

    tDelay = t + runInterval;
}

/*
 * Messages consist of the letter S followed by
 * ,int(0-1),int(0-59),int(0-59)
 * S,0,20,5 (pump,nozzleInterval,nozzleDuration)
 */
void PUMP::serialControl()
{
    char c = Serial.read();
    switch (c)
    {
    case 'S':
    {
        pumpOn = Serial.parseInt();
        nozzleInterval = Serial.parseInt();
        nozzleDuration = Serial.parseInt();

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
        if (runProgram == 1)
        {
            runProgram = 0;
            break;
        }
        if (runProgram == 0)
        {
            runProgram = 1;
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
        t = now();
        tDelay = runInterval;
        break;
    }
    }
}

void PUMP::scheduleFromUser()
{
    // Change every hour
    int x = hour(t); // 0-23
    int z = pumpScheduleIndex[0][x];

    pumpOn = pumpSchedule[z][0];
    nozzleInterval = pumpSchedule[z][1];
    nozzleDuration = pumpSchedule[z][2];
}

/**
 * Turn the pump on or off depending on the value of the global variable pumpOn.
 */
void PUMP::setPump()
{
    if (pumpOn == 1)
    {
        Relay.RelayOnOff(pump_relay_pin, true);
    }
    if (pumpOn == 0)
    {
        Relay.RelayOnOff(pump_relay_pin, false);
    }
}

void PUMP::setNozzle()
{
    if (nozzleDuration == 0)
    {
        digitalWrite(NOZZLE, LOW);
    }
}

String PUMP::timeReport()
{
    String timeString;

    timeString = "Time(dd:hh:mm:ss):      ";
    timeString = timeString + day(t);
    timeString = timeString + ":";
    timeString = timeString + hour(t);
    timeString = timeString + ":";
    timeString = timeString + minute(t);
    timeString = timeString + ":";
    timeString = timeString + second(t);
    timeString = timeString + "\n";

    return timeString;
}

void PUMP::serialReport()
{
    String reportValues;

    reportValues = "Settings:\n";
    reportValues = "Pump(0=off, 1=on):      ";
    reportValues = reportValues + pumpOn;
    reportValues = reportValues + "\nMinutes between sprays: ";
    reportValues = reportValues + nozzleInterval;
    reportValues = reportValues + "\nSeconds of spray:       ";
    reportValues = reportValues + nozzleDuration;

    reportValues = "-- Report ----\n" + timeReport() + reportValues;
    Serial.println(reportValues);
}

PUMP pump;