#include "accumulatedata.hpp"

AccumulateData::AccumulateData(GreenHouseConfig& config,
                               ProjectConfig& deviceConfig,
                               LDR& ldr,
                               TowerTemp& towertemp,
                               Humidity& humidity,
                               WaterLevelSensor& waterlevelsensor,
                               NetworkNTP& ntp,
                               BaseMQTT& mqtt)
    : _config(config),
      _deviceConfig(deviceConfig),
      _ldr(ldr),
      _towertemp(towertemp),
      _humidity(humidity),
      _waterLevelSensor(waterlevelsensor),
      _waterLevelPercentage(_waterLevelSensor),
      _ntp(ntp),
      _floatSensorSerializer(),
      _stringSensorSerializer(),
      _vectorStringSensorSerializer(),
      _vectorFloatSensorSerializer(),
      _humiditySerializer(),
      _mqtt(mqtt),
      _gatherDataTimer(60000),
      _maxTemp(100),
      _numTempSensors(0),
      _sensors{
          &_ldr,
          &_waterLevelSensor,
          &_waterLevelPercentage,
      } {}

AccumulateData::~AccumulateData() {}

void AccumulateData::begin() {}

//* Collect the data
/**
 * @brief Accumulate Data to send from sensors and store in json.
 * @note This function accumulates all sensor data and stores it in the main
 * data structure.
 * @parameters: None
 * @return void
 */
void AccumulateData::loop() {
  if (_gatherDataTimer.ding()) {
    _ntp.ntpLoop();

    std::string json = "{";

    log_d("[Accumulate Data]: Gathering data...");
    _ntp.accept(_stringSensorSerializer);

    log_d("[Accumulate Data]: Tower");
    _towertemp.accept(_vectorFloatSensorSerializer);

    log_d("[Accumulate Data]: Humidity");
    _humidity.accept(_humiditySerializer);

    //* Pass the data to the mqtt client
    log_d("[Accumulate Data]: Humidity");

    log_d("[Accumulate Data]: %s", _mqtt.mqttConnected() ? "true" : "false");

    if (_mqtt.mqttConnected()) {
      log_d("[Accumulate Data]: NTPTimer MQTT");
      _mqtt.dataHandler(_stringSensorSerializer.sensorName,
                        _stringSensorSerializer.value);
      log_d("[Accumulate Data]: Tower MQTT");
      _mqtt.dataHandler(_vectorFloatSensorSerializer.sensorName,
                        _vectorFloatSensorSerializer.serializedData);
      log_d("[Accumulate Data]: Humidity MQTT");
      _mqtt.dataHandler(_humiditySerializer.sensorName,
                        _humiditySerializer.serializedData);
    }

    //* build the json string
    json.append(Helpers::format_string(
        "%s", _stringSensorSerializer.serializedData.c_str()));

    json.append(",");

    //* Serialize the temperature vector
    json.append(Helpers::format_string(
        "%s", _vectorFloatSensorSerializer.serializedData.c_str()));

    json.append(",");

    //* Serialize the humidity
    json.append(Helpers::format_string(
        "%s", _humiditySerializer.serializedData.c_str()));

    json.append(",");

    //* Generate JSON for the sensors
    for (auto it = _sensors.begin(); it != _sensors.end(); ++it) {
      //* serialize the data
      log_d("[Accumulate Data]: Sensors");
      (*it)->accept(_floatSensorSerializer);

      //* add the data to the json string
      json.append(Helpers::format_string(
          "%s", _floatSensorSerializer.serializedData.c_str()));

      if (it != _sensors.end() - 1)
        json.append(",");

      //* Pass the data to the mqtt client
      if (_mqtt.mqttConnected())
        log_d("[Accumulate Data]: Sensors MQTT");
      _mqtt.dataHandler(_floatSensorSerializer.sensorName,
                        _floatSensorSerializer.value);
    }

    json.append("}");
    _deviceConfig.getDeviceDataJson().deviceJson.assign(json);

    log_d("[Data Json Document]: %s", json.c_str());
    _gatherDataTimer.start();
  }
}
