#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP

#include <vector>

//* Data Struct
#include <local/data/config/config.hpp>
#include "local/Serializers/SensorSerializer/sensorserializer.hpp"
#include "local/data/visitor.hpp"

//*  Sensor Includes
#include <local/io/sensors/humidity/humidity.hpp>
#include <local/io/sensors/light/ldr.hpp>
#include <local/io/sensors/temperature/towertemp.hpp>
#include <local/io/sensors/water_level/waterlevelsensor.hpp>

//* Network Includes
#include <local/network/ntp/ntp.hpp>

class AccumulateData {
  LDR& ldr;
  TowerTemp& towertemp;
  Humidity& humidity;
  WaterLevelSensor& waterlevelsensor;
  NetworkNTP& ntp;
  ProjectConfig& deviceConfig;
  GreenHouseConfig& config;
  SensorSerializer sensorSerializer;

  timeObj generateJSONTimer;
  timeObj gatherDataTimer;

  // Stack Data to send
  int _maxTemp;
  int _numTempSensors;

  std::vector<Element<Visitor<SensorInterface<float>>>*> sensors;

 public:
  AccumulateData(LDR& ldr,
                 TowerTemp& towertemp,
                 Humidity& humidity,
                 WaterLevelSensor& waterlevelsensor,
                 NetworkNTP& ntp,
                 ProjectConfig& deviceConfig,
                 GreenHouseConfig& config);
  virtual ~AccumulateData();

  void begin();
  void loop();
  bool accumulateData();
};
#endif
