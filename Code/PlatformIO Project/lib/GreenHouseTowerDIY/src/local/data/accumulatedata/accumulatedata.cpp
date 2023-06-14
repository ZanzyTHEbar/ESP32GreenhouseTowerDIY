#include "accumulatedata.hpp"

AccumulateData::AccumulateData(LDR& ldr,
                               TowerTemp& towertemp,
                               Humidity& humidity,
                               WaterLevelSensor& waterlevelsensor,
                               NetworkNTP& ntp,
                               ProjectConfig& deviceConfig,
                               GreenHouseConfig& config,
                               BaseMQTT& mqtt)
    : _ldr(ldr),
      _towertemp(towertemp),
      _humidity(humidity),
      _waterLevelSensor(waterlevelsensor),
      _waterLevelPercentage(_waterLevelSensor),
      _ntp(ntp),
      _deviceConfig(deviceConfig),
      _config(config),
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
  _ntp.ntpLoop();

  if (_config.getEnabledFeatures().temp_features !=
      GreenHouseConfig::TempFeatures_t::TEMP_C) {
    _towertemp.getTempF();
  } else {
    _towertemp.getTempC();
  }

  std::string json = "{";

  if (_gatherDataTimer.ding()) {
    _ntp.accept(_stringSensorSerializer);
    _towertemp.accept(_vectorFloatSensorSerializer);
    _humidity.accept(_humiditySerializer);

    //* Pass the data to the mqtt client
    _mqtt.dataHandler(_stringSensorSerializer.sensorName,
                      _stringSensorSerializer.value);
    _mqtt.dataHandler(_vectorFloatSensorSerializer.sensorName,
                      _vectorFloatSensorSerializer.value);

    _mqtt.dataHandler(_humiditySerializer.sensorName,
                      _humiditySerializer.value);

    //* build the json string
    json.append(Helpers::format_string(
        "%s", _stringSensorSerializer.serializedData.c_str()));

    //* Serialize the temperature vector
    json.append(Helpers::format_string(
        "%s", _vectorFloatSensorSerializer.serializedData.c_str()));

    //* Serialize the humidity
    json.append(Helpers::format_string(
        "%s", _humiditySerializer.serializedData.c_str()));

    //* Generate JSON for the sensors
    for (auto& sensor : _sensors) {
      //* serialize the data
      sensor->accept(_floatSensorSerializer);

      //* add the data to the json string
      json.append(Helpers::format_string(
          "%s", _floatSensorSerializer.serializedData.c_str()));

      //* Pass the data to the mqtt client
      _mqtt.dataHandler(_floatSensorSerializer.sensorName,
                        _floatSensorSerializer.value);
    }
    json.append("}");
    _deviceConfig.getDeviceDataJson().deviceJson.assign(json);

    log_d("[Data Json Document]: %s", json.c_str());
    _gatherDataTimer.start();
  }
}
