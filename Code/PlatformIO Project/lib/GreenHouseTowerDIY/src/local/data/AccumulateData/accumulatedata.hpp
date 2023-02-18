#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP
#include <ArduinoJson.h>
#include <vector>

// data Struct
#include <local/data/BackgroundTasks/taskManager.hpp>
#include <local/data/config/config.hpp>
// Temp local/io/Sensors
#include <local/io/sensors/temperature/towertemp.hpp>
// Humidity local/io/Sensors
#include <local/io/sensors/humidity/Humidity.hpp>
// Water Level local/io/Sensors
#include <local/io/sensors/water_level/waterlevelsensor.hpp>
// Time stamp
#include <local/network/ntp/ntp.hpp>

class AccumulateData : public IObserver<ObserverEvent::CustomEvents>,
                       public timeObj
{
public:
  AccumulateData(GreenHouseConfig *configManager,
                 NetworkNTP *ntp,
                 TowerTemp *tower_temp,
                 Humidity *humidity,
                 WaterLevelSensor *waterLevelSensor);
  virtual ~AccumulateData();

  void loop();
  void update(ObserverEvent::CustomEvents event);
  bool accumulateData();

private:
  // Stack Data to send
  int _maxTemp;
  int _numTempSensors;

  GreenHouseConfig *configManager;
  NetworkNTP *ntp;
  TowerTemp *tower_temp;
  Humidity *humidity;
  WaterLevelSensor *waterLevelSensor;
};
#endif
