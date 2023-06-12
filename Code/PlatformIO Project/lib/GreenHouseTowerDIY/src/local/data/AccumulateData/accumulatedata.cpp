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
      _sensorSerializer(),
      _stringSerializer(),
      _mqtt(mqtt),
      _gatherDataTimer(60000),
      _maxTemp(100),
      _numTempSensors(0),
      data({
          .sensors =
              {
                  {&_ldr, 0},
                  {&_waterLevelSensor, 0},
                  {&_waterLevelPercentage, 0},
              },
          .ntpTime = std::string(),
      }) {}

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

  std::string json = "{";

  if (_gatherDataTimer.ding()) {
    _ntp.accept(_stringSerializer);
    data.ntpTime.assign(_stringSerializer.value);

    //* build the json string
    json.append(
        Helpers::format_string("%s", _stringSerializer.serializedData.c_str()));

    for (auto& sensor : data.sensors) {
      //* serialize the data
      sensor.first->accept(_sensorSerializer);

      //* Pass the data to the mqtt client
      sensor.first->accept(_mqtt);

      //* add the data to the json string
      json.append(Helpers::format_string(
          "%s", _sensorSerializer.serializedData.c_str()));
      sensor.second = _sensorSerializer.value;
    }
    json.append("}");
    _deviceConfig.getDeviceDataJson().deviceJson.assign(json);

    log_d("[Data Json Document]: %s", json.c_str());
    _gatherDataTimer.start();
  }
}
