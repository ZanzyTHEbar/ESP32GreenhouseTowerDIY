#include "pHsensor.hpp"

PHSENSOR::_pHcommandMap mapping;
PHSENSOR::_phmap phmap;

PHSENSOR::PHSENSOR() : _pH{std::make_shared<Gravity_pH>(PH_SENSOR_PIN)},
                       _pHTopic(PH_TOPIC),
                       _pHOutTopic(PH_OUT_TOPIC),
                       _inputstring_array{0},
                       _phDnPIN(PH_DN_PIN),
                       _phUpPIN(PH_UP_PIN),
                       _doseTimeSm(DOSE_TIME_SM),
                       _doseTimeMed(DOSE_TIME_MED),
                       _doseTimeLg(DOSE_TIME_LG),
                       _inputstring(""),
                       _input_string_complete(false) {}

PHSENSOR::~PHSENSOR()
{
    // mapping.erase(mapping.begin(), mapping.end());
    // phmap.erase(phmap.begin(), phmap.end());µ

    mapping.clear();
    phmap.clear();

    delete &mapping;
    delete &phmap;
}

void PHSENSOR::begin()
{
    pinMode(_phUpPIN, OUTPUT);
    pinMode(_phDnPIN, OUTPUT);
    // pH probe calibration serial commands
    Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the circuit to those respective values"));
    Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
    if (!_pH->begin())
    {
        log_i("Failed to load EEPROM");
        return;
    }

    mapping["CAL,4"] = &Gravity_pH::cal_low;
    mapping["CAL,7"] = &Gravity_pH::cal_mid;
    mapping["CAL,10"] = &Gravity_pH::cal_high;
    mapping["CAL,CLEAR"] = &Gravity_pH::cal_clear;

    phmap["PHUP"] = &setPHUpPin;
    phmap["PHDN"] = &setPHDnPin;
}

void PHSENSOR::serialEvent()
{                                              // if the hardware serial port_0 receives a char
    _inputstring = Serial.readStringUntil(13); // read the string until we see a <CR>
    _input_string_complete = true;             // set the flag used to tell if we have received a completed string from the PC
}

/* Legacy Function */
/* void PHSENSOR::parse_cmd(const char *string)
{
    if (strcmp(string, "CAL,7") == 0)
    {
        _pH->cal_mid();
        log_i("MID CALIBRATED");
    }
    else if (strcmp(string, "CAL,4") == 0)
    {
        _pH->cal_low();
        log_i("LOW CALIBRATED");
    }
    else if (strcmp(string, "CAL,10") == 0)
    {
        _pH->cal_high();
        log_i("HIGH CALIBRATED");
    }
    else if (strcmp(string, "CAL,CLEAR") == 0)
    {
        _pH->cal_clear();
        log_i("CALIBRATION CLEARED");
    }
    else if (strcmp(string, "PHUP") == 0)
    {
        digitalWrite(_phUpPIN, HIGH);
        my_delay(_doseTimeSm);
        digitalWrite(_phUpPIN, LOW); // Dose of pH up
    }
    else if (strcmp(string, "PHDN") == 0)
    {
        digitalWrite(_phDnPIN, HIGH);
        my_delay(_doseTimeSm);
        digitalWrite(_phDnPIN, LOW); // Dose of pH down
    }
} */

void PHSENSOR::setPHUpPin()
{
    digitalWrite(_phUpPIN, HIGH);
    custom_delay(_doseTimeSm);
    digitalWrite(_phUpPIN, LOW);
}

void PHSENSOR::setPHDnPin()
{
    digitalWrite(_phDnPIN, HIGH);
    custom_delay(_doseTimeSm);
    digitalWrite(_phDnPIN, LOW);
}

void PHSENSOR::parse_cmd_lookup(std::string index)
{
    _pHcommandMap::iterator it = mapping.find(index);
    _phmap::iterator it_2 = phmap.find(index);
    if (it != mapping.end())
    {
        Gravity_pH m(*_pH);
        (m.*(it->second))();
        // mapping.at(index);
    }
    else if (it_2 != phmap.end())
    {
        (this->*(it_2->second))();
    }
    else
    {
        log_e("Command not found");
        return;
    }
}

float PHSENSOR::getPH()
{
    return _pH->read_ph();
}

void PHSENSOR::phSensorLoop()
{
    if (_input_string_complete == true)
    {                                                     // check if data received
        _inputstring.toCharArray(_inputstring_array, 30); // convert the string to a char array
        std::string temp = _inputstring_array;            // create a string from the char array
        parse_cmd_lookup(temp);                           // send data to pars_cmd function
        _input_string_complete = false;                   // reset the flag used to tell if we have received a completed string from the PC
        _inputstring = "";                                // clear the string
    }
    Serial.println(getPH());
}

void PHSENSOR::eventListener(const char *topic, const uint8_t *payload, uint16_t length)
{
    // The message is for the pH sensor.
    // The message is in the format:
    // <pH>:<pH_value>
    String result;
    for (int i = 0; i < length; i++)
    {
        log_i("%s", (char)payload[i]);
        result += (char)payload[i];
    }

    // Act on the message
    if (result.equalsIgnoreCase("UP MED"))
    {
        log_i("Adjust pH UP a medium amount!");
        digitalWrite(_phUpPIN, HIGH);
        my_delay(_doseTimeMed);
        digitalWrite(_phUpPIN, LOW);
    }
    else if (result.equalsIgnoreCase("DOWN MED"))
    {
        log_i("Adjusting pH DOWN a medium amount!");
        digitalWrite(_phDnPIN, HIGH);
        my_delay(_doseTimeMed);
        digitalWrite(_phDnPIN, LOW);
    }
    else if (result.equalsIgnoreCase("UP SM"))
    {
        log_i("Adjust pH UP a small amount!");
        digitalWrite(_phUpPIN, HIGH);
        my_delay(_doseTimeSm);
        digitalWrite(_phUpPIN, LOW);
    }
    else if (result.equalsIgnoreCase("DOWN SM"))
    {
        log_i("Adjusting pH DOWN a small amount!");
        digitalWrite(_phDnPIN, HIGH);
        my_delay(_doseTimeSm);
        digitalWrite(_phDnPIN, LOW);
    }
    else if (result.equalsIgnoreCase("UP LG"))
    {
        log_i("Adjust pH UP a large amount!");
        digitalWrite(_phUpPIN, HIGH);
        my_delay(_doseTimeLg);
        digitalWrite(_phUpPIN, LOW);
    }
    else if (result.equalsIgnoreCase("DOWN LG"))
    {
        log_i("Adjusting pH DOWN a large amount!");
        digitalWrite(_phDnPIN, HIGH);
        my_delay(_doseTimeLg);
        digitalWrite(_phDnPIN, LOW);
    }
}

PHSENSOR phsensor;
