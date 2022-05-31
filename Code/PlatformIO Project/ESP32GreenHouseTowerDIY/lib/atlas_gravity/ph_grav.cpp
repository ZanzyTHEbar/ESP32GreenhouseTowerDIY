
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "ph_grav.h"
#include "EEPROM.h"

Gravity_pH::Gravity_pH(uint8_t pin)
{
  this->pin = pin;
  this->EEPROM_offset = (pin)*EEPROM_SIZE_CONST;
  // to lay the calibration parameters out in EEPROM we map their locations to the analog pin numbers
  // we assume a maximum size of EEPROM_SIZE_CONST for every struct we're saving
}

bool Gravity_pH::begin()
{
#if defined(ESP8266) || defined(ESP32)
  EEPROM.begin(1024);
#endif
  if ((EEPROM.read(this->EEPROM_offset) == magic_char) && (EEPROM.read(this->EEPROM_offset + sizeof(uint8_t)) == GRAV_PH))
  {
    EEPROM.get(this->EEPROM_offset, pH);
    return true;
  }
  return false;
}

float Gravity_pH::read_voltage()
{
  float voltage_mV = 0;
  for (int i = 0; i < volt_avg_len; ++i)
  {
#if defined(ESP8266) || defined(ESP32)
    voltage_mV += analogRead(this->pin) / 4096.0 * 3300.0;
#else
    voltage_mV += analogRead(this->pin) / 1024.0 * 5000.0;
#endif // ESP32
  }
  voltage_mV /= volt_avg_len;
  return voltage_mV;
}

float Gravity_pH::read_ph(float voltage_mV)
{
  if (voltage_mV > pH.mid_cal)
  { // high voltage = low ph
    return 7.0 - 3.0 / (this->pH.low_cal - this->pH.mid_cal) * (voltage_mV - this->pH.mid_cal);
  }
  else
  {
    return 7.0 - 3.0 / (this->pH.mid_cal - this->pH.high_cal) * (voltage_mV - this->pH.mid_cal);
  }
}

float Gravity_pH::read_ph()
{
  return (read_ph(read_voltage()));
}

void Gravity_pH::cal_mid(float voltage_mV)
{
  this->pH.mid_cal = voltage_mV;
  EEPROM.put(this->EEPROM_offset, pH);
#if defined(ESP8266) || defined(ESP32)
  EEPROM.commit();
#endif
}

void Gravity_pH::cal_mid()
{
  cal_mid(read_voltage());
}

void Gravity_pH::cal_low(float voltage_mV)
{
  this->pH.low_cal = voltage_mV;
  EEPROM.put(this->EEPROM_offset, pH);
#if defined(ESP8266) || defined(ESP32)
  EEPROM.commit();
#endif
}

void Gravity_pH::cal_low()
{
  cal_low(read_voltage());
}

void Gravity_pH::cal_high(float voltage_mV)
{
  this->pH.high_cal = voltage_mV;
  EEPROM.put(this->EEPROM_offset, pH);
#if defined(ESP8266) || defined(ESP32)
  EEPROM.commit();
#endif
}

void Gravity_pH::cal_high()
{
  cal_high(read_voltage());
}

void Gravity_pH::cal_clear()
{
  this->pH.mid_cal = 1500;
  this->pH.low_cal = 2030;
  this->pH.high_cal = 975;
  EEPROM.put(this->EEPROM_offset, pH);
}
