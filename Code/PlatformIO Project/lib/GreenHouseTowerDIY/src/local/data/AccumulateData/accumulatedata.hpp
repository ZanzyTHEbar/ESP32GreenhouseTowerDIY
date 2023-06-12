#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP

#include <unordered_map>

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
  LDR& _ldr;
  TowerTemp& _towertemp;
  Humidity& _humidity;
  WaterLevelSensor& _waterLevelSensor;
  WaterLevelPercentage _waterLevelPercentage;
  NetworkNTP& _ntp;
  ProjectConfig& _deviceConfig;
  GreenHouseConfig& _config;
  SensorSerializer _sensorSerializer;
  StringSerializer _stringSerializer;

  timeObj _generateJSONTimer;
  timeObj _gatherDataTimer;

  // Stack Data to send
  int _maxTemp;
  int _numTempSensors;

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

  struct SensorData {
    std::unordered_map<Element<Visitor<SensorInterface<float>>>*, float>
        sensors;
    std::string ntpTime;
  };

  SensorData data;
};
#endif
