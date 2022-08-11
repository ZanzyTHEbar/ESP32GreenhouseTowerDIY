#include "serialhandler.hpp"

void pumpSerialHandler();
void pHSerialHandler();

SerialHandler::SerialHandler() {}

SerialHandler::~SerialHandler() {}

void SerialHandler::begin()
{
    /*
     * Setup callbacks for SerialCommand commands:
     * PU+COMMAND=X -> Write handler
     * Messages consist of the letter S followed by
     * ,int(0-1),int(0-59),int(0-59)
     * S,0,20,5 (pump,_nozzleInterval,_nozzleDuration)
     */
    serialManager.addWriteManager("PU+CONTROL", &pumpSerialHandler); // pump control

    /*
     * Setup callbacks for SerialCommand commands:
     * PH+CALIB -> Execute handler
     */
    serialManager.addWriteManager("PH+CALIB", &pHSerialHandler); // triggers ph serial event listener to listen for pH commands
}

void SerialHandler::loop()
{
    serialManager.loop(); /* Process data from serial port each iteration */
}

/*
  Usage examples :
    - AT+COMMAND=1<CR>
    - AT+COMMAND=0<CR>
*/
void pumpSerialHandler()
{
    char *arg;
    bool ok = false;
    arg = serialManager.next(); /* Get the next argument from the SerialCommand object buffer */
    if (arg != NULL)            /* Check if argument exists */
    {
        ok = true;
        switch (*arg)
        {
        case 'S':
        {
            pump.pump_data._pumpOn = Serial.parseInt();
            pump.pump_data._nozzleInterval = Serial.parseInt();
            pump.pump_data._nozzleDuration = Serial.parseInt();

            pump.setPump();
            pump.setNozzle();
            break;
        }
        case 'R':
        {
            pump.serialReport();
            break;
        }
        case 'Q':
        {
            if (pump.pump_data._runProgram == 1)
            {
                pump.pump_data._runProgram = 0;
                break;
            }

            if (pump.pump_data._runProgram == 0)
            {
                pump.pump_data._runProgram = 1;
                break;
            }
            break;
        }
        case 'T':
        {
            int hour = Serial.parseInt();                                                            // First valid integer
            int min = Serial.parseInt();                                                             // Second valid integer
            int day = Serial.parseInt();                                                             // Third valid integer
            setTime(hour, min, 0, day, networkntp.getMonth().toInt(), networkntp.getYear().toInt()); // hour,min,sec,day,month,year
            pump.pump_data._t_ = now();
            pump.pump_data._tDelay = pump.pump_data._runInterval;
            break;
        }
        default:
        { // Unexpected value
            ok = false;
            break;
        }
        }
    }
    ok ? log_i("[SerialHandler] : PHHandler - Data ok") : log_e("[SerialHandler] : \r\nPHHandler - ERROR\r\n");
}

void pHSerialHandler()
{
    char *arg;
    bool ok = false;
    arg = serialManager.next(); /* Get the next argument from the SerialCommand object buffer */
    if (arg != NULL)            /* Check if argument exists */
    {
        ok = true;
        std::string temp(arg);
        phsensor.parse_cmd_lookup(temp);
    }

    ok ? log_i("[SerialHandler] : PHHandler - Data ok") : log_e("[SerialHandler] : \r\nPHHandler - ERROR\r\n");
    // ok = false;
}