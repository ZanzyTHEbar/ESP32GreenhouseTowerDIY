#include "pHsensor.hpp"

PHSENSOR::PHSENSOR()
    : pH(PH_SENSOR_PIN),
      _pHTopic(PH_TOPIC),
      _pHOutTopic(PH_OUT_TOPIC),
      _phDnPIN(PH_DN_PIN),
      _phUpPIN(PH_UP_PIN),
      _doseTimeSm(DOSE_TIME_SM),
      _doseTimeMed(DOSE_TIME_MED),
      _doseTimeLg(DOSE_TIME_LG),
      _inputstring(std::string()),
      _input_string_complete(false)
{
  pinMode(_phUpPIN, OUTPUT);
  pinMode(_phDnPIN, OUTPUT);
}

PHSENSOR::~PHSENSOR() {}

void PHSENSOR::begin()
{
  // pH probe calibration serial commands
  Serial.println(
      F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the "
        "circuit to those respective values"));
  Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
  if (pH.begin())
  {
    log_i("Loaded EEPROM");
  }
}

void PHSENSOR::serialEvent()
{ // if the hardware serial port_0 receives a char
  _inputstring.assign(Serial.readStringUntil(13)
                          .c_str()); // read the string until we see a <CR>
  _input_string_complete = true;     // set the flag used to tell if we have
                                     // received a completed string from the PC
}

void PHSENSOR::parse_cmd(const char *string)
{
  strupr((char *)string);
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
    Network_Utilities::my_delay(_doseTimeSm);
    digitalWrite(_phUpPIN, LOW); // Dose of pH up
  }
  else if (strcmp(string, "PHDN") == 0)
  {
    digitalWrite(_phDnPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeSm);
    digitalWrite(_phDnPIN, LOW); // Dose of pH down
  }
}

void PHSENSOR::setPH()
{
  phContainer.ph = pH.read_ph();
}
/**
 * @brief Get the PH object from the pH sensor
 *  This function is used to get the pH value from the pH sensor.
 * @return const PHSENSOR::PH_t*
 */
const PHSENSOR::PH_t *PHSENSOR::getPH()
{
  const PH_t *phContainer = &this->phContainer;
  return phContainer;
}

void PHSENSOR::phSensorLoop()
{
  if (_input_string_complete == true)
  {                                  // check if data received
    parse_cmd(_inputstring.c_str()); // send data to pars_cmd function
    _input_string_complete = false;  // reset the flag used to tell if we have
                                     // received a completed string from the PC
    _inputstring = "";               // clear the string
  }
  getPH();
  log_i("%.3f", pH.read_ph());
}

void PHSENSOR::eventListener(const char *topic,
                             const uint8_t *payload,
                             uint16_t length)
{
  // The message is for the pH sensor.
  // The message is in the format:
  // <pH>:<pH_value>
  std::string result;
  for (int i = 0; i < length; i++)
  {
    log_i("%s", (char)payload[i]);
    result += (char)payload[i];
  }

  // Convert to lower case using lambda
  std::for_each(result.begin(), result.end(),
                [](char &c)
                { c = ::tolower(c); });

  // Act on the message
  if (result == "up med")
  {
    log_i("Adjust pH UP a medium amount!");
    digitalWrite(_phUpPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeMed);
    digitalWrite(_phUpPIN, LOW);
  }
  else if (result == "down med")
  {
    log_i("Adjusting pH DOWN a medium amount!");
    digitalWrite(_phDnPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeMed);
    digitalWrite(_phDnPIN, LOW);
  }
  else if (result == "up sm")
  {
    log_i("Adjust pH UP a small amount!");
    digitalWrite(_phUpPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeSm);
    digitalWrite(_phUpPIN, LOW);
  }
  else if (result == "down sm")
  {
    log_i("Adjusting pH DOWN a small amount!");
    digitalWrite(_phDnPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeSm);
    digitalWrite(_phDnPIN, LOW);
  }
  else if (result == "up lg")
  {
    log_i("Adjust pH UP a large amount!");
    digitalWrite(_phUpPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeLg);
    digitalWrite(_phUpPIN, LOW);
  }
  else if (result == "down lg")
  {
    log_i("Adjusting pH DOWN a large amount!");
    digitalWrite(_phDnPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeLg);
    digitalWrite(_phDnPIN, LOW);
  }
}

PHSENSOR phsensor;
