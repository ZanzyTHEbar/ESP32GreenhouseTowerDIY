#include "accumulatedata.hpp"

AccumulateData::AccumulateData(PHSENSOR* phsensor,
                               BH1750* bh1750,
                               LDR* ldr,
                               TowerTemp* towertemp,
                               Humidity* humidity,
                               WaterLevelSensor* waterlevelsensor,
                               NetworkNTP* ntp,
                               I2C_RelayBoard* relayboard,
                               GreenHouseConfig* deviceConfig)
    : _maxTemp(100),
      _numTempSensors(0),
      phsensor(phsensor),
      bh1750(bh1750),
      ldr(ldr),
      towertemp(towertemp),
      humidity(humidity),
      waterlevelsensor(waterlevelsensor),
      ntp(ntp),
      relayboard(relayboard),
      deviceConfig(deviceConfig),
      generateJSONTimer(30000),
      gatherDataTimer(15000) {}

AccumulateData::~AccumulateData() {}

void AccumulateData::loop() {
  ntp->NTPLoop();
  if (gatherDataTimer.ding()) {
    //* Collect the data
#if USE_PH_SENSOR
    phsensor->phSensorLoop();
#endif
#if USE_SHT31_SENSOR
    humidity->readSHT31();
#endif
#if USE_DHT_SENSOR
    humidity->readDHT();
#endif
#if USE_BH1750_SENSOR
    bh1750->getLux();
#endif

#if USE_LDR_SENSOR
    ldr->getLux();
#endif

#if USE_WATER_LEVEL_SENSOR_ULTRASONIC
    waterlevelsensor->readWaterLevelUltraSonic();
#endif

#if USE_WATER_LEVEL_SENSOR_INFRARED
    waterlevelsensor->readWaterLevelInfrared();
#endif

#if USE_WATER_LEVEL_SENSOR_PRESSURE
    waterlevelsensor->readWaterLevelPressure();
#endif

    log_i("Reading Tower Temp");
    if (towertemp->getSensorCount() > 0) {
      log_i("Inside of Temp Sensor Check");
      towertemp->getTempC();
      waterlevelsensor->readWaterLevelUltraSonic();
    }
    gatherDataTimer.start();
  }
}

//******************************************************************************
// * Function: Accumulate Data to send from sensors and store in json
// * Description: This function accumulates all sensor data and stores it in the
// * main data structure.
// * Parameters: None
// * Return: bool - true if data was accumulated, false if not
//******************************************************************************
bool AccumulateData::accumulateData() {
  // TODO: Add all sensors to the JSON data structure
  if (generateJSONTimer.ding()) {
    _numTempSensors = towertemp->getSensorCount();
    // assign the data to the data structure
    std::string json = "";
    StaticJsonDocument<1024> jsonDoc;
    jsonDoc["id"] = ntp->getFullDate();
    jsonDoc["timestamp"] = ntp->getTimeStamp();
    jsonDoc["max_temp"] = _maxTemp;
    jsonDoc["num_temp_sensors"] = _numTempSensors;

    jsonDoc["water_level_liters"] = waterlevelsensor->result.water_level;
    jsonDoc["water_level_percentage"] =
        waterlevelsensor->result.water_level_percentage;
#if USE_SHT31_SENSOR
    switch (humidity->humiditySensorsActive) {
      case 0:
        break;
      case 1:
        jsonDoc["humidity_sht31"] = humidity->result.humidity_sht31;
        jsonDoc["humidity_temp_sht31"] = humidity->result.temp_sht31;
        break;
      case 2:
        jsonDoc["humidity_sht31_2"] = humidity->result.humidity_sht31_2;
        jsonDoc["humidity_temp_sht31_2"] = humidity->result.temp_sht31_2;
        break;
      case 3:
        jsonDoc["humidity_sht31"] = humidity->result.humidity_sht31;
        jsonDoc["humidity_temp_sht31"] = humidity->result.temp_sht31;
        jsonDoc["humidity_sht31_2"] = humidity->result.humidity_sht31_2;
        jsonDoc["humidity_temp_sht31_2"] = humidity->result.temp_sht31_2;
        break;
      default:
        break;
    }

    jsonDoc["humidity_sht31_average"] = humidity->stackHumidity();
    jsonDoc["humidity_temp_sht31_average"] = humidity->averageStackTemp();
#endif  // USE_SHT31_SENSOR

#if USE_DHT_SENSOR
    jsonDoc["humidity_dht"] = humidity->result.humidity;
    jsonDoc["humidity_temp_dht"] = humidity->result.temp;
#endif  // USE_DHT_SENSOR

    JsonArray temp_sensor_data = jsonDoc.createNestedArray("temp_sensors");
    for (int i = 0; i < _numTempSensors; i++) {
      temp_sensor_data.add(towertemp->temp_sensor_results.temp[i]);
    }
    if (serializeJson(jsonDoc, json) == 0) {
      log_e("[Data Json Document]: Failed to write to file");
      generateJSONTimer.start();
      return false;
    }
    if (json.length() < 1) {
      log_e("[Data Json Document]: Failed to write to file");
      generateJSONTimer.start();
      return false;
    }

    deviceConfig->getDeviceDataJson()->deviceJson.assign(json);
    serializeJsonPretty(jsonDoc, json);
    log_d("[Data Json Document]: %s", json.c_str());
    generateJSONTimer.start();
  }
  return true;
}

void AccumulateData::update(ObserverEvent::CustomEvents event) {
  switch (event) {
    case ObserverEvent::CustomEvents::accumulateData:
      accumulateData();
      break;
    default:
      break;
  }
}
