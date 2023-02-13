#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP
#include <ArduinoJson.h>
#include <vector>

// data Struct
#include <data/StateManager/StateManager.hpp>
#include <data/config/project_config.hpp>
// Temp local/io/Sensors
#include <local/io/sensors/temperature/towertemp.hpp>
// Humidity local/io/Sensors
#include <local/io/sensors/humidity/Humidity.hpp>
// Water Level local/io/Sensors
#include <local/io/sensors/water_level/waterlevelsensor.hpp>
// Time stamp
#include <local/network/ntp/ntp.hpp>

class AccumulateData {
 public:
  AccumulateData(ProjectConfig* configManager,
                 NetworkNTP* ntp,
#if USE_GOOGLE_SHEETS
                 NetworkHTTP* http,
#endif  // USE_GOOGLE_SHEETS
                 TowerTemp* tower_temp,
                 Humidity* humidity,
                 WaterLevelSensor* waterLevelSensor);
  virtual ~AccumulateData();

  void loop();
  bool accumulateData();

 private:
  // Stack Data to send
  int _maxTemp;
  int _numTempSensors;

  ProjectConfig* configManager;
  NetworkNTP* ntp;
#if USE_GOOGLE_SHEETS
  NetworkHTTP* http;
#endif  // USE_GOOGLE_SHEETS
  TowerTemp* tower_temp;
  Humidity* humidity;
  WaterLevelSensor* waterLevelSensor;
};
#endif
