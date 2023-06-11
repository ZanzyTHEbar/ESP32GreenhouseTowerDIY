#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP
#include <ArduinoJson.h>
#include <vector>

//* Data Struct
#include <local/data/config/config.hpp>

//*  Sensor Includes
#include <local/io/sensors/humidity/Humidity.hpp>
#include <local/io/sensors/light/bh1750.hpp>
#include <local/io/sensors/light/ldr.hpp>
#include <local/io/sensors/temperature/towertemp.hpp>
#include <local/io/sensors/water_level/waterlevelsensor.hpp>

//* Network Includes
#include <local/network/ntp/ntp.hpp>

class AccumulateData {
  BH1750& bh1750;
  LDR& ldr;
  TowerTemp& towertemp;
  Humidity& humidity;
  WaterLevelSensor& waterlevelsensor;
  NetworkNTP& ntp;
  ProjectConfig& deviceConfig;

  timeObj generateJSONTimer;
  timeObj gatherDataTimer;

  // Stack Data to send
  int _maxTemp;
  int _numTempSensors;

 public:
  AccumulateData(BH1750& bh1750,
                 LDR& ldr,
                 TowerTemp& towertemp,
                 Humidity& humidity,
                 WaterLevelSensor& waterlevelsensor,
                 NetworkNTP& ntp,
                 ProjectConfig& deviceConfig);
  virtual ~AccumulateData();

  void loop();
  bool accumulateData();
};
#endif
