#include "accumulatedata.hpp"

// TODO: Add all the sensors here

AccumulateData::AccumulateData(GreenHouseConfig *configManager, NetworkNTP *ntp,
                               TowerTemp *tower_temp, Humidity *humidity,
                               WaterLevelSensor *waterLevelSensor)
    : _maxTemp(100), _numTempSensors(0), configManager(configManager), ntp(ntp),
      tower_temp(tower_temp), humidity(humidity),
      waterLevelSensor(waterLevelSensor) {}

AccumulateData::~AccumulateData() {}

void AccumulateData::loop()
{
  ntp->NTPLoop();
// Initialize the libraries and collect the data
#if USE_SHT31_SENSOR
  humidity->ReadSensor();
#endif // USE_SHT31_SENSOR

#if USE_DHT_SENSOR
  humidity->readDHT();
#endif // USE_DHT_SENSOR
  log_i("Reading Tower Temp");
  if (tower_temp->getSensorCount() > 0)
  {
    log_i("Inside of Temp Sensor Check");
    tower_temp->getTempC();
    waterLevelSensor->readWaterLevelUltraSonic();
  }
}

//******************************************************************************
// * Function: Accumulate Data to send from sensors and store in json
// * Description: This function accumulates all sensor data and stores it in the
// main data structure.
// * Parameters: None
// * Return: bool - true if data was accumulated, false if not
//******************************************************************************/
bool AccumulateData::accumulateData()
{
  // TODO: add a timer to this fucntion to make sure it doesn't run too often
  _numTempSensors = tower_temp->getSensorCount();
  // assign the data to the data structure
  std::string json = "";
  StaticJsonDocument<1024> jsonDoc;
  jsonDoc["id"] = ntp->getFullDate();
  jsonDoc["timestamp"] = ntp->getTimeStamp();
  jsonDoc["max_temp"] = _maxTemp;
  jsonDoc["num_temp_sensors"] = _numTempSensors;
  // jsonDoc["flow_rate"] = _config.flow_rate;
  // jsonDoc["flow_rate_sensor_temp"] = _config.flow_rate_sensor_temp;
  jsonDoc["water_level_liters"] = waterLevelSensor->result.water_level;
  jsonDoc["water_level_percentage"] =
      waterLevelSensor->result.water_level_percentage;
#if USE_SHT31_SENSOR
  switch (humidity->_HUMIDITY_SENSORS_ACTIVE)
  {
  case 0:
    break;
  case 1:
    jsonDoc["humidity_sht31"] = humidity.result.humidity_sht31;
    jsonDoc["humidity_temp_sht31"] = humidity.result.temp_sht31;
    break;
  case 2:
    jsonDoc["humidity_sht31_2"] = humidity.result.humidity_sht31_2;
    jsonDoc["humidity_temp_sht31_2"] = humidity.result.temp_sht31_2;
    break;
  case 3:
    jsonDoc["humidity_sht31"] = humidity.result.humidity_sht31;
    jsonDoc["humidity_temp_sht31"] = humidity.result.temp_sht31;
    jsonDoc["humidity_sht31_2"] = humidity.result.humidity_sht31_2;
    jsonDoc["humidity_temp_sht31_2"] = humidity.result.temp_sht31_2;
    break;
  default:
    break;
  }
  jsonDoc["humidity_sht31_average"] = humidity->StackHumidity();
  jsonDoc["humidity_temp_sht31_average"] = humidity->AverageStackTemp();
#endif // USE_SHT31_SENSOR
#if USE_DHT_SENSOR
  jsonDoc["humidity_dht"] = humidity->result.humidity;
  jsonDoc["humidity_temp_dht"] = humidity->result.temp;
#endif // USE_DHT_SENSOR
  JsonArray temp_sensor_data = jsonDoc.createNestedArray("temp_sensors");
  for (int i = 0; i < _numTempSensors; i++)
  {
    temp_sensor_data.add(tower_temp->temp_sensor_results.temp[i]);
  }
  if (serializeJson(jsonDoc, json) == 0)
  {
    log_e("[Data Json Document]: Failed to write to file");
    return false;
  }
  if (json.length() > 0)
  {
    configManager->getDeviceDataJson()->deviceJson.assign(json);
    serializeJsonPretty(jsonDoc, json);
    log_d("[Data Json Document]: %s", json.c_str());
    return true;
  }
  return false;
}

void AccumulateData::update(ObserverEvent::CustomEvents event)
{
  switch (event)
  {
  case ObserverEvent::CustomEvents::accumulateData:
    accumulateData();
    break;
  default:
    break;
  }
}
