#include "pHsensor.hpp"

Gravity_pH pH = Gravity_pH(PH_SENSOR_PIN);

PHSENSOR::PHSENSOR()
{
}

PHSENSOR::~PHSENSOR()
{
    _pHTopic = PH_TOPIC;
    _pHOutTopic = PH_OUT_TOPIC;
    _phDnPIN = PH_DN_PIN;
    _phUpPIN = PH_UP_PIN;
    _doseTimeSm = DOSE_TIME_SM;
    _doseTimeMed = DOSE_TIME_MED;
    _doseTimeLg = DOSE_TIME_LG;
}

void PHSENSOR::phSensorSetup()
{
    pinMode(_phUpPIN, OUTPUT);
    pinMode(_phDnPIN, OUTPUT);
    // pH probe calibration serial commands
    Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the circuit to those respective values"));
    Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
    if (pH.begin())
    {
        log_i("Loaded EEPROM");
    }
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

void PHSENSOR::parse_cmd(char *string)
{
    strupr(string);
    if (strcmp(string, "CAL,7") == 0)
    {
        pH.cal_mid();
        log_i("MID CALIBRATED");
    }
    else if (strcmp(string, "CAL,4") == 0)
    {
        pH.cal_low();
        log_i("LOW CALIBRATED");
    }
    else if (strcmp(string, "CAL,10") == 0)
    {
        pH.cal_high();
        log_i("HIGH CALIBRATED");
    }
    else if (strcmp(string, "CAL,CLEAR") == 0)
    {
        pH.cal_clear();
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
}

void PHSENSOR::getPH()
{
    cfg.config.pH = pH.read_ph();
}
