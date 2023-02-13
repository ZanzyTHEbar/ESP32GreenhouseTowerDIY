#include "Humidity.hpp"

std::unordered_map<Humidity::_HUMIDITY_SENSORS_ACTIVE, std::string>
    Humidity::humidity_sensors_map = {
        {Humidity::_HUMIDITY_SENSORS_ACTIVE::HUMIDITY_SENSORS_ACTIVE_NONE,
         "Humidity Sensor Setup Failed - initialised to NONE"},
        {Humidity::_HUMIDITY_SENSORS_ACTIVE::HUMIDITY_SENSORS_ACTIVE_SHT31,
         "Humidity Sensor Setup - initialised to SHT31"},
        {Humidity::_HUMIDITY_SENSORS_ACTIVE::HUMIDITY_SENSORS_ACTIVE_SHT31_2,
         "Humidity Sensor Setup - initialised to SHT31_2"},
        {Humidity::_HUMIDITY_SENSORS_ACTIVE::HUMIDITY_SENSORS_ACTIVE_BOTH,
         "Humidity Sensor Setup - initialised to BOTH"},
        {Humidity::_HUMIDITY_SENSORS_ACTIVE::HUMIDITY_SENSORS_ACTIVE_DHT,
         "Humidity Sensor Setup - initialised to DHT"},
        {Humidity::_HUMIDITY_SENSORS_ACTIVE::HUMIDITY_SENSORS_ACTIVE_DHT_SHT31,
         "Humidity Sensor Setup - initialised to DHT_SHT31"},
        {Humidity::_HUMIDITY_SENSORS_ACTIVE::
             HUMIDITY_SENSORS_ACTIVE_DHT_SHT31_2,
         "Humidity Sensor Setup - initialised to DHT_SHT31_2"},
};  // end of map

Humidity::Humidity()
    : _delayS(0),
      _humiditySensorsActive(HUMIDITY_SENSORS_ACTIVE_NONE),
#if USE_DHT_SENSOR
      dht{std::make_shared<DHT_Unified>(DHTPIN, DHTTYPE)}
#endif  // USE_DHT_SENSOR
#if USE_SHT31_SENSOR
      ,
      _enableHeater(false),
      _loopCnt(0),
      sht31{std::make_shared<Adafruit_SHT31>()},
      sht31_2{std::make_shared<Adafruit_SHT31>()},
#endif  // USE_SHT31_SENSOR
{
}

Humidity::~Humidity() {}

void Humidity::begin() {
  auto hum_iter = humidity_sensors_map.find(setup());
  if (hum_iter != humidity_sensors_map.end()) {
    Serial.println(F("Found humidity sensor"));
    Serial.println(F(hum_iter->second.c_str()));
  } else {
    Serial.println(F("Humidity Sensor Setup Failed - Unknown Error"));
  }
}

void checkISNAN(const char* msg, float data) {
  char buffer[sizeof(msg) + sizeof(data) + 43];
  snprintf(buffer, sizeof(buffer),
           (!isnan(data)) ? "%s: %.3f"
                          : "failed to read water level sensor %s: %.3f",
           msg, data);
  log_d("%s", buffer);
}

/******************************************************************************
 * Function: Setup Humidity Sensors
 * Description: This function is used to initialise the humidity sensors and
 *their respective heaters Parameters: None Return: Enum
 *_HUMIDITY_SENSORS_ACTIVE
 ******************************************************************************/
Humidity::_HUMIDITY_SENSORS_ACTIVE Humidity::setup() {
#if USE_DHT_SENSOR
  // Initialize the DHT sensor.
  dht->begin();
  log_i("DHT Sensor connected!");
  _humiditySensorsActive = HUMIDITY_SENSORS_ACTIVE_DHT;
  // Print temperature sensor details.
  sensor_t sensor;
  dht->temperature().getSensor(&sensor);
  log_d("------------------------------------");
  log_d("Temperature Sensor");
  log_d("Sensor Type: %s", sensor.name);
  log_d("Driver Ver: %d", sensor.version);
  log_d("Unique ID:  %d", sensor.sensor_id);
  log_d("Max Value: %.3f °C", sensor.max_value);
  log_d("Min Value: %.3f °C", sensor.min_value);
  log_d("Resolution: %.3f °C", sensor.resolution);
  log_d("------------------------------------");
  // Print humidity sensor details.
  dht->humidity().getSensor(&sensor);
  log_d("Humidity Sensor");
  log_d("Sensor Type: %s", sensor.name);
  log_d("Driver Ver: %d", sensor.version);
  log_d("Unique ID:  %d", sensor.sensor_id);
  log_d("Max Value: %.3f °C", sensor.max_value);
  log_d("Min Value: %.3f °C", sensor.min_value);
  log_d("Resolution: %.3f °C", sensor.resolution);
  log_d("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  _delayS = sensor.min_delay / 1000;
  log_d("Delay: %d ms", _delayS);
  log_d("------------------------------------");
  log_d("");
#endif  // USE_DHT_SENSOR

#if USE_SHT31_SENSOR
  log_d("SHT31 Sensors Setup Beginning....");
  // Set to 0x45 for alternate i2c address
  if (!sht31->begin(0x44) && !sht31_2->begin(0x45)) {
    log_d("Couldn't find SHT31 sensors");
    log_d(
        "SHT31 Sensors Setup did not complete successfully, check your wiring "
        "or the addresses and try again");
    _humiditySensorsActive = HUMIDITY_SENSORS_ACTIVE_NONE;
  } else if (!sht31->begin(0x44) && sht31_2->begin(0x45)) {
    log_d("Found 1 SHT31 Sensor");
    _humiditySensorsActive = HUMIDITY_SENSORS_ACTIVE_SHT31;
  } else if (!sht31_2->begin(0x45) && sht31->begin(0x44)) {
    log_d("Found 2 SHT31 Sensor");
    _humiditySensorsActive = HUMIDITY_SENSORS_ACTIVE_SHT31_2;
  } else {
    log_d("SHT31 Sensors Setup Complete");
    _humiditySensorsActive = HUMIDITY_SENSORS_ACTIVE_BOTH;
  }
  delay(2L);  // delay in between reads for stability
#endif        // USE_SHT31_SENSOR
  return _humiditySensorsActive;
}

#if USE_DHT_SENSOR
Humidity::Hum Humidity::readDHT() {
  // Delay between measurements.
  // TODO: This is a blocking delay, which is not ideal.  We should be able to
  // use the millis() function to check if the delay has passed.
  delay(_delayS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht->temperature().getEvent(&event);
  result.temp = event.temperature;
  checkISNAN("Temperature", event.temperature);
  // Get humidity event and print its value.
  dht->humidity().getEvent(&event);
  checkISNAN("Humidity", event.relative_humidity);
  result.humidity = event.relative_humidity;
  return result;
}
#endif  // USE_DHT_SENSOR

#if USE_SHT31_SENSOR
bool Humidity::checkHeaterEnabled() {
  switch (_humiditySensorsActive) {
    case HUMIDITY_SENSORS_ACTIVE_NONE:
      return false;
      break;
    case HUMIDITY_SENSORS_ACTIVE_SHT31: {
      bool _sensor1 = sht31->isHeaterEnabled();
      bool heaterenabled = false;
      if (_loopCnt >= 30) {
        _enableHeater = !_enableHeater;
        sht31->heater(_enableHeater);
        log_d("Heater Enabled State: %s",
              _enableHeater ? "Enabled" : "Disabled");
        if (_sensor1) {
          log_d("Sensor 1 Heater ENABLED");
          heaterenabled = true;
        } else {
          log_d("Sensor 1 Heater Disabled");
          heaterenabled = false;
        }
        _loopCnt = 0;
      }
      _loopCnt++;
      return heaterenabled;
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_SHT31_2: {
      bool _sensor2 = sht31_2->isHeaterEnabled();
      bool heaterenabled = false;
      if (_loopCnt >= 30) {
        _enableHeater = !_enableHeater;
        sht31_2->heater(_enableHeater);
        log_d("Heater Enabled State: %s",
              _enableHeater ? "Enabled" : "Disabled");
        if (_sensor2) {
          log_d("Sensors have Heater ENABLED");
          heaterenabled = true;
        } else {
          log_d("Sensor 1 Heater Disabled");
          heaterenabled = false;
        }
        _loopCnt = 0;
      }
      _loopCnt++;
      return heaterenabled;
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_BOTH: {
      bool _sensor1 = sht31->isHeaterEnabled();
      bool _sensor2 = sht31_2->isHeaterEnabled();
      bool heaterenabled = false;
      if (_loopCnt >= 30) {
        _enableHeater = !_enableHeater;
        sht31->heater(_enableHeater);
        sht31_2->heater(_enableHeater);
        log_d("Heater Enabled State: %s",
              _enableHeater ? "Enabled" : "Disabled");
        if (_sensor1 != _sensor2) {
          log_d("Sensors have Heater ENABLED");
          heaterenabled = true;
        } else {
          log_d("Sensor 1 Heater Disabled");
          heaterenabled = false;
        }
        _loopCnt = 0;
      }
      _loopCnt++;
      return heaterenabled;
      break;
    }
    default:  // Should never get here
      return false;
      break;
  }
}

/******************************************************************************
 * Function: Average Stack Humidity Temp
 * Description: This function is used to average the Temp of the stack - from
 *the temp sensors built into the Humidity Sensors Parameters: None Return:
 *float Note: This function MUST be called after the ReadSensor function
 ******************************************************************************/
float Humidity::AverageStackTemp() {
  switch (_humiditySensorsActive) {
    case HUMIDITY_SENSORS_ACTIVE_NONE: {
      float stack_temp = 0;
      return stack_temp;  // return 0 if no sensors are active
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_SHT31: {
      float stack_temp = result.temp_sht31;
      return stack_temp;  // Only one sensor - return the value of that sensor
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_SHT31_2: {
      float stack_temp = result.temp_sht31_2;
      return stack_temp;  // Only one sensor - return the value of that sensor
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_BOTH: {
      float stack_temp = result.temp_sht31 + result.temp_sht31_2;
      return stack_temp / 2;  // Read the _temperature from the sensor and
                              // average the two sensors.
      break;
    }
    default: {
      break;
    }
  }
}

/******************************************************************************
 * Function: Average Stack Humidity
 * Description: This function is used to average the humidity of the stack
 * Parameters: None
 * Return: float
 * Note: This function MUST be called after the ReadSensor function
 ******************************************************************************/
float Humidity::StackHumidity() {
  switch (_humiditySensorsActive) {
    case HUMIDITY_SENSORS_ACTIVE_NONE: {
      float stack_humidity = 0;
      return stack_humidity;  // return 0 if no sensors are active
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_SHT31: {
      float stack_humidity = result.humidity;
      return stack_humidity;  // Only one sensor - return the value of that
                              // sensor
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_SHT31_2: {
      float stack_humidity = result.humidity_2;
      return stack_humidity;  // Only one sensor - return the value of that
                              // sensor
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_BOTH: {
      float stack_humidity = result.humidity + result.humidity_2;
      return stack_humidity / 2;  // Read the _humidity from the sensor and
                                  // average the two sensors.
      break;
    }
    default: {
      break;
    }
  }
}

/******************************************************************************
 * Function: Read Humidity Sensors
 * Description: This function is used to read the humidity of the stack sensors
 * Parameters: None
 * Return: float array
 ******************************************************************************/
Humidity::Hum Humidity::ReadSensor() {
  switch (_humiditySensorsActive) {
    case HUMIDITY_SENSORS_ACTIVE_NONE: {
      result.temp_sht31 = 0;
      result.humidity_sht31 = 0;
      result.temp_sht31_2 = 0;
      result.humidity_sht31_2 = 0;
      return result;
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_SHT31: {
      float temp = sht31->readTemperature();
      float hum = sht31->readHumidity();
      // check if 'is not a number'
      checkISNAN("Temp", temp);
      checkISNAN("Humidity", hum);
      my_delay(0.1L);  // delay in between reads for stability

      // Toggle heater enabled state every 30 seconds
      // An ~3.0 degC _temperature increase can be noted when heater is enabled
      // This is needed due to the high operating humidity of the system
      checkHeaterEnabled();
      result.temp_sht31 = temp;
      result.humidity_sht31 = hum;
      return result;
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_SHT31_2: {
      float temp_2 = sht31_2->readTemperature();
      float hum_2 = sht31_2->readHumidity();

      checkISNAN("Temp_2", temp_2);
      checkISNAN("Hum_2", hum_2);

      my_delay(0.1L);  // delay in between reads for stability

      // Toggle heater enabled state every 30 seconds
      // An ~3.0 degC _temperature increase can be noted when heater is enabled
      // This is needed due to the high operating humidity of the system
      checkHeaterEnabled();
      result.temp_sht31_2 = temp_2;
      result.humidity_sht31_2 = hum_2;
      return result;
      break;
    }
    case HUMIDITY_SENSORS_ACTIVE_BOTH: {
      float temp_1 = sht31->readTemperature();
      float temp_2 = sht31_2->readTemperature();
      checkISNAN("Temp_1", temp_1);
      checkISNAN("Temp_2", temp_2);

      // check if 'is not a number'
      float hum_1 = sht31->readHumidity();
      float hum_2 = sht31_2->readHumidity();
      checkISNAN("Hum_1", hum_1);
      checkISNAN("Hum_2", hum_2);
      my_delay(1L);  // delay in between reads for stability
      result = {temp_1, hum_1, temp_2, hum_2};
      return result;
      break;
    }
    default:  // Should never get here
      result.temp_sht31 = 0;
      result.humidity_sht31 = 0;
      result.temp_sht31_2 = 0;
      result.humidity_sht31_2 = 0;
      return result;
      break;
  }
}
#endif  // USE_HUMIDITY_SENSOR
