#include "pHsensor.hpp"

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
                       _input_string_complete(false),
                       _pHcommandMap{0, 0},
                       _pHcustomcommandsMap{0, 0} {}

PHSENSOR::~PHSENSOR()
{

    // delete _pHcommandMap;
    // delete _pHcustomcommandsMap;

    // _pHcommandMap.clear();
    // _pHcustomcommandsMap.clear();

    _pHcommandMap.erase(_pHcommandMap.begin(), _pHcommandMap.end());
    _pHcustomcommandsMap.erase(_pHcustomcommandsMap.begin(), _pHcustomcommandsMap.end());
}

void PHSENSOR::begin()
{
    pinMode(_phUpPIN, OUTPUT);
    pinMode(_phDnPIN, OUTPUT);
    // pH probe calibration serial commands
    Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the circuit to those respective values"));
    Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
    if (_pH->begin())
    {
        log_i("Loaded EEPROM");
    }

    // create a map of the commands and their corresponding functions
    //_pHcommandMap.emplace(new std::string("CAL,4"), std::bind(static_cast<void (Gravity_pH::*)()>(&Gravity_pH::cal_low)));
    //_pHcommandMap.emplace(new std::string("CAL,4"), resolve(&Gravity_pH::cal_low));
    //_pHcommandMap.emplace(new std::string("CAL,7"), std::bind(static_cast<void (Gravity_pH::*)()>(&Gravity_pH::cal_mid)));
    //_pHcommandMap.emplace(new std::string("CAL,10"), std::bind(static_cast<void (Gravity_pH::*)()>(&Gravity_pH::cal_high)));
    //_pHcommandMap.emplace(new std::string("CAL,CLEAR"), std::bind(&Gravity_pH::cal_clear));
    //_pHcommandMap.emplace(&std::string("CAL,4"), &Gravity_pH::cal_low);
    //_pHcommandMap.emplace(&std::string("CAL,7"), &Gravity_pH::cal_mid);
    //_pHcommandMap.emplace(&std::string("CAL,10"), &Gravity_pH::cal_high);
    //_pHcommandMap.emplace(&std::string("CAL,CLEAR"), &Gravity_pH::cal_clear);

    _pHcustomcommandsMap.emplace(new std::string("PHUP"), std::bind(&PHSENSOR::setPHPin, this, _phUpPIN, &_doseTimeSm));
    _pHcustomcommandsMap.emplace(new std::string("PHDN"), std::bind(&PHSENSOR::setPHPin, this, _phDnPIN, &_doseTimeSm));
    _pHcustomcommandsMap.emplace(new std::string("PHUP_MED"), std::bind(&PHSENSOR::setPHPin, this, _phUpPIN, &_doseTimeMed));
    _pHcustomcommandsMap.emplace(new std::string("PHDN_MED"), std::bind(&PHSENSOR::setPHPin, this, _phDnPIN, &_doseTimeMed));
    _pHcustomcommandsMap.emplace(new std::string("PHUP_LG"), std::bind(&PHSENSOR::setPHPin, this, _phUpPIN, &_doseTimeLg));
    _pHcustomcommandsMap.emplace(new std::string("PHDN_LG"), std::bind(&PHSENSOR::setPHPin, this, _phDnPIN, &_doseTimeLg));

    _pHcommandMap.emplace(new std::string("CAL,10"), [this]()
                          { _pH->cal_high(); });
    _pHcommandMap.emplace(new std::string("CAL,7"), [this]()
                          { _pH->cal_mid(); });
    _pHcommandMap.emplace(new std::string("CAL,4"), [this]()
                          { _pH->cal_low(); });
    _pHcommandMap.emplace(new std::string("CAL,CLEAR"), [this]()
                          { _pH->cal_clear(); });
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

void PHSENSOR::setPHPin(byte pin, int *doseTime)
{
    digitalWrite(pin, HIGH);
    custom_delay(*doseTime);
    digitalWrite(pin, LOW);
}

void PHSENSOR::parse_cmd_lookup(std::string *index)
{
    std::map<std::string *, std::shared_ptr<Gravity_pH>>::iterator it;
    std::map<std::string *, std::function<PHSENSOR>>::iterator it_2;
    it = _pHcommandMap.find(index);
    it_2 = _pHcustomcommandsMap.find(index);
    if (it != _pHcommandMap.end())
    {
        _pHcommandMap.at(index);
    }
    else if (it_2 != _pHcustomcommandsMap.end())
    {
        _pHcustomcommandsMap.at(index);
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
        parse_cmd_lookup(&temp);                          // send data to pars_cmd function
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
