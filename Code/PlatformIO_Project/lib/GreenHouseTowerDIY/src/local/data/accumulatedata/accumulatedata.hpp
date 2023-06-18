#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP

//* Network Includes
#include "local/network/mqtt/basic/basicmqtt.hpp"

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
  GreenHouseConfig& _config;
  ProjectConfig& _deviceConfig;
  LDR& _ldr;
  TowerTemp& _towertemp;
  Humidity& _humidity;
  WaterLevelSensor& _waterLevelSensor;
  WaterLevelPercentage _waterLevelPercentage;
  NetworkNTP& _ntp;
  SensorSerializer<float> _floatSensorSerializer;
  SensorSerializer<std::string> _stringSensorSerializer;
  SensorSerializer<std::vector<std::string>> _vectorStringSensorSerializer;
  SensorSerializer<Temp_Array_t> _vectorFloatSensorSerializer;
  SensorSerializer<Humidity_Return_t> _humiditySerializer;
  BaseMQTT& _mqtt;
  timeObj _gatherDataTimer;

  // Stack Data to send
  int _maxTemp;
  int _numTempSensors;
  std::vector<Element<Visitor<SensorInterface<float>>>*> _sensors;

 public:
  AccumulateData(GreenHouseConfig& config,
                 ProjectConfig& deviceConfig,
                 LDR& ldr,
                 TowerTemp& towertemp,
                 Humidity& humidity,
                 WaterLevelSensor& waterlevelsensor,
                 NetworkNTP& ntp,
                 BaseMQTT& mqtt);
  virtual ~AccumulateData();

  void begin();
  void loop();
};
#endif
