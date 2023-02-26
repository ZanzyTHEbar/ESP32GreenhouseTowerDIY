#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP
#include <ArduinoJson.h>
#include <vector>

//* Data Struct
#include <local/data/BackgroundTasks/taskManager.hpp>
#include <local/data/config/config.hpp>

//*  Sensor Includes
#include <local/io/sensors/humidity/Humidity.hpp>
#include <local/io/sensors/light/bh1750.hpp>
#include <local/io/sensors/light/ldr.hpp>
#include <local/io/sensors/pH/pHsensor.hpp>
#include <local/io/sensors/temperature/towertemp.hpp>
#include <local/io/sensors/water_level/waterlevelsensor.hpp>

//* IO Includes
#include <local/io/Relays/relays.hpp>

//* Time stamp
#include <local/network/ntp/ntp.hpp>

class AccumulateData : public IObserver<ObserverEvent::CustomEvents>,
                       public timeObj {
  PHSENSOR* phsensor;
  BH1750* bh1750;
  LDR* ldr;
  TowerTemp* towertemp;
  Humidity* humidity;
  WaterLevelSensor* waterlevelsensor;
  NetworkNTP* ntp;
  I2C_RelayBoard* relayboard;
  GreenHouseConfig* deviceConfig;

  // Stack Data to send
  int _maxTemp;
  int _numTempSensors;

 public:
  AccumulateData(PHSENSOR* phsensor,
                 BH1750* bh1750,
                 LDR* ldr,
                 TowerTemp* towertemp,
                 Humidity* humidity,
                 WaterLevelSensor* waterlevelsensor,
                 NetworkNTP* ntp,
                 I2C_RelayBoard* relayboard,
                 GreenHouseConfig* deviceConfig);
  virtual ~AccumulateData();

  void loop();
  void update(ObserverEvent::CustomEvents event);
  bool accumulateData();
};
#endif
