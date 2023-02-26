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
      _input_string_complete(false) {
  pinMode(_phUpPIN, OUTPUT);
  pinMode(_phDnPIN, OUTPUT);
}

PHSENSOR::~PHSENSOR() {}

void PHSENSOR::begin() {
  // pH probe calibration serial commands
  Serial.println(
      F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the "
        "circuit to those respective values"));
  Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
  if (pH.begin()) {
    log_i("Loaded EEPROM");
  }
}

/**
 * @brief The pH sensor Serial event listener
 * @note This function is used to listen for serial events for the pH sensor.
 * @return void
 */
void PHSENSOR::serialEvent() {
  if (Serial.available()) {
    // if the hardware serial port_0 receives a char
    _inputstring.assign(Serial.readStringUntil(13)
                            .c_str());  // read the string until we see a <CR>
    _input_string_complete = true;      // set the flag used to tell if we have
                                    // received a completed string from the PC
  }
}

/**
 * @brief Set the PH object from the pH sensor
 * @note This function is used to set the pH value from the pH sensor. Called in
 * the accumulate data class.
 */
void PHSENSOR::setPH() {
  phContainer.ph = pH.read_ph();
}
/**
 * @brief Get the PH object from the pH sensor
 *  This function is used to get the pH value from the pH sensor.
 * @return const PHSENSOR::PH_t*
 */
const PHSENSOR::PH_t* PHSENSOR::getPH() {
  const PH_t* phContainer = &this->phContainer;
  return phContainer;
}

/**
 * @brief Handle the entire pH sensor loop
 * @note This function is used to handle the entire pH sensor loop. This
 * function is called in the main loop.
 */
void PHSENSOR::readPH() {
  serialEvent();
  if (_input_string_complete == true) {  // check if data received
    parse_cmd(_inputstring.c_str());     // send data to parse_cmd function
    _input_string_complete = false;  // reset the flag used to tell if we have
                                     // received a completed string from the PC
    _inputstring = "";               // clear the string
  }
  setPH();
  log_i("%.3f", pH.read_ph());
}

/**
 * @brief Parse the command from the serial monitor
 * @note This function is used to parse the command from the serial monitor. The
 * command is used to calibrate the pH sensor.
 */
void PHSENSOR::parse_cmd(const std::string& string) {
  std::string result = string;

  // Convert to lower case using lambda
  std::for_each(result.begin(), result.end(),
                [](char& c) { c = ::tolower(c); });

  if (result == "cal,7") {
    pH.cal_mid();
    log_i("MID CALIBRATED");
  } else if (result == "cal,4") {
    pH.cal_low();
    log_i("LOW CALIBRATED");
  } else if (result == "cal,10") {
    pH.cal_high();
    log_i("HIGH CALIBRATED");
  } else if (result == "cal,clear") {
    pH.cal_clear();
    log_i("CALIBRATION CLEARED");
  } else if (string == "phup") {
    digitalWrite(_phUpPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeSm);
    digitalWrite(_phUpPIN, LOW);  // Dose of pH up
  } else if (string == "phdn") {
    digitalWrite(_phDnPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeSm);
    digitalWrite(_phDnPIN, LOW);  // Dose of pH down
  }
}

/**
 * @brief Event listener for the pH sensor
 * @note This function is used to listen for events from the MQTT broker.
 * Inorder to adjust the pH of the water, the pH sensor must be calibrated.
 * @param topic The topic of the event
 * @param payload The payload of the event
 * @param length The length of the payload
 * @return void
 */
void PHSENSOR::eventListener(const std::string& result) {
  // The message is for the pH sensor.
  // The message is in the format:
  // <pH>:<pH_value>

  // Convert to lower case using lambda
  std::for_each(result.begin(), result.end(),
                [](char& c) { c = ::tolower(c); });

  // Act on the message
  if (result == "up:med") {
    log_i("Adjust pH UP a medium amount!");
    digitalWrite(_phUpPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeMed);
    digitalWrite(_phUpPIN, LOW);
  } else if (result == "down:med") {
    log_i("Adjusting pH DOWN a medium amount!");
    digitalWrite(_phDnPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeMed);
    digitalWrite(_phDnPIN, LOW);
  } else if (result == "up:sm") {
    log_i("Adjust pH UP a small amount!");
    digitalWrite(_phUpPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeSm);
    digitalWrite(_phUpPIN, LOW);
  } else if (result == "down:sm") {
    log_i("Adjusting pH DOWN a small amount!");
    digitalWrite(_phDnPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeSm);
    digitalWrite(_phDnPIN, LOW);
  } else if (result == "up:lg") {
    log_i("Adjust pH UP a large amount!");
    digitalWrite(_phUpPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeLg);
    digitalWrite(_phUpPIN, LOW);
  } else if (result == "down:lg") {
    log_i("Adjusting pH DOWN a large amount!");
    digitalWrite(_phDnPIN, HIGH);
    Network_Utilities::my_delay(_doseTimeLg);
    digitalWrite(_phDnPIN, LOW);
  }
}
