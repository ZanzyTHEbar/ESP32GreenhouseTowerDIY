#include "pHsensor.hpp"

PHSENSOR::PHSENSOR() : _pH{std::make_shared<Gravity_pH>(PH_SENSOR_PIN)} {}

PHSENSOR::~PHSENSOR()
{
    // mapping.erase(mapping.begin(), mapping.end());
    // phmap.erase(phmap.begin(), phmap.end());

    mapping.clear();
    phmap.clear();

    delete &mapping;
    delete &phmap;
}

void PHSENSOR::begin()
{
    pinMode(_phData._phUpPIN, OUTPUT);
    pinMode(_phData._phDnPIN, OUTPUT);

    /* _phData = {
        .id = 0,
        ._phDnPIN = PH_DN_PIN,
        ._phUpPIN = PH_UP_PIN,
        ._doseTimeSm = DOSE_TIME_SM,
        ._doseTimeMed = DOSE_TIME_MED,
        ._doseTimeLg = DOSE_TIME_LG,
        ._pHTopic = PH_TOPIC,
        ._pHOutTopic = PH_OUT_TOPIC,
        ._inputstring = "",
        ._input_string_complete = false,
        ._inputstring_array = {0},
    }; */

    ph_data_map["id"] = ph_Data_t{
        .id = 0,
        ._phDnPIN = PH_DN_PIN,
        ._phUpPIN = PH_UP_PIN,
        ._doseTimeSm = DOSE_TIME_SM,
        ._doseTimeMed = DOSE_TIME_MED,
        ._doseTimeLg = DOSE_TIME_LG,
        ._pHTopic = PH_TOPIC,
        ._pHOutTopic = PH_OUT_TOPIC,
        ._inputstring = "",
        ._input_string_complete = false,
        ._inputstring_array = {0},
    };

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

    phmap.emplace("PHUP", std::bind(&PHSENSOR::setPHPin, this, &_phData._phUpPIN, &_phData._doseTimeSm, true));
    phmap.emplace("PHDN", std::bind(&PHSENSOR::setPHPin, this, &_phData._phDnPIN, &_phData._doseTimeSm, true));
    phmap.emplace("PHUP,MED", std::bind(&PHSENSOR::setPHPin, this, &_phData._phUpPIN, &_phData._doseTimeMed, true));
    phmap.emplace("PHDN,MED", std::bind(&PHSENSOR::setPHPin, this, &_phData._phDnPIN, &_phData._doseTimeMed, true));
    phmap.emplace("PHUP,LG", std::bind(&PHSENSOR::setPHPin, this, &_phData._phUpPIN, &_phData._doseTimeLg, true));
    phmap.emplace("PHDN,LG", std::bind(&PHSENSOR::setPHPin, this, &_phData._phDnPIN, &_phData._doseTimeLg, true));
}

void PHSENSOR::setPHPin(uint8_t *pin, int *time, bool state)
{
    digitalWrite(*time, state);
    custom_delay(*time);
    digitalWrite(*pin, !state);
    float test = getPH();
}

void PHSENSOR::parse_cmd_lookup(std::string &index)
{
    PhCommandMap_t::const_iterator it = mapping.find(index);
    PhMap_t::const_iterator it_2 = phmap.find(index);
    if (it != mapping.end())
    {
        (*_pH.*(it->second))();
    }
    else if (it_2 != phmap.end())
    {
        it_2->second; //! FIXME: fix this
        //(this->*(it_2->second))();
    }
    else
    {
        log_i("Command not found");
    }

    auto &key_it = it->first;
    // auto& value_it = it->second;

    auto &key_it_2 = it_2->first;
    // auto& value_it_2 = it_2->second;

    log_i("Command %s executed", key_it.c_str());
}

float PHSENSOR::getPH()
{
    return _pH->read_ph();
}

/* void PHSENSOR::serialEvent()
{                                                        // if the hardware serial port_0 receives a char
    _phData._inputstring = Serial.readStringUntil('\r'); // read the string until we see a <CR>
    _phData._input_string_complete = true;               // set the flag used to tell if we have received a completed string from the PC
} */

/* void PHSENSOR::phSensorLoop()
{
    if (_phData._input_string_complete == true)
    {                                                     // check if data received
        _phData._inputstring.toCharArray(_phData._inputstring_array, 30); // convert the string to a char array
        std::string temp = _phData._inputstring_array;                     // create a string from the char array
        parse_cmd_lookup(temp);                           // send data to pars_cmd function
        _phData._input_string_complete = false;                   // reset the flag used to tell if we have received a completed string from the PC
        _phData._inputstring = "";                                // clear the string
    }
    Serial.println(getPH());
} */

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

