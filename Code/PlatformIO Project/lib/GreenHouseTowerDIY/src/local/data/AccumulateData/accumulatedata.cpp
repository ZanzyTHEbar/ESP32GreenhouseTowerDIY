#include "accumulatedata.hpp"

AccumulateData::AccumulateData(LDR& ldr,
                               TowerTemp& towertemp,
                               Humidity& humidity,
                               WaterLevelSensor& waterlevelsensor,
                               NetworkNTP& ntp,
                               ProjectConfig& deviceConfig,
                               GreenHouseConfig& config)
    : ldr(ldr),
      towertemp(towertemp),
      humidity(humidity),
      waterlevelsensor(waterlevelsensor),
      ntp(ntp),
      deviceConfig(deviceConfig),
      config(config),
      sensorSerializer(),
      stringSerializer(),
      gatherDataTimer(60000),
      _maxTemp(100),
      _numTempSensors(0),
      sensors{&ldr} {}

AccumulateData::~AccumulateData() {}

void AccumulateData::begin() {}

//* Collect the data
/**
 * @brief Accumulate Data to send from sensors and store in json. \n
 * @note This function accumulates all sensor data and stores it in the main
 * data structure.
 * @parameters: None
 * @return void
 */
void AccumulateData::loop() {
  ntp.ntpLoop();

  std::string json = "{";

  if (gatherDataTimer.ding()) {
    //* build the json string
    ntp.accept(stringSerializer);
    json.append(
        Helpers::format_string("%s", stringSerializer.serializedData.c_str()));

    for (auto& sensor : sensors) {
      //* serialize the data
      sensor->accept(sensorSerializer);
      //* add the data to the json string
      json.append(Helpers::format_string(
          "%s", sensorSerializer.serializedData.c_str()));
    }
    json.append("}");
    deviceConfig.getDeviceDataJson().deviceJson.assign(json);

    log_d("[Data Json Document]: %s", json.c_str());
    gatherDataTimer.start();
  }
}
