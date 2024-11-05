#pragma once

#include <ports/api.hpp>
#include <ports/logger.hpp>
#include <ports/pump.hpp>
#include <ports/sensor.hpp>

#include <algorithm>
#include <vector>

class GardenController {
 private:
  std::vector<SensorInterface&> sensors;
  PumpInterface& pump;
  LoggerInterface& logger;
  APIInterface& api;

 public:
  GardenController(PumpInterface& p,
                   LoggerInterface& log,
                   APIInterface& apiInterface)
      : pump(p), logger(log), api(apiInterface) {}

  void run();

  void addSensor(SensorInterface& sensor) { sensors.push_back(sensor); }

  void removeSensor(uint64_t id) {
    sensors.erase(std::remove_if(sensors.begin(), sensors.end(),
                                 [id](const SensorInterface& s) {
                                   return s.getID() == id;
                                 }),
                  sensors.end());
  }

  void logEvent(const std::string& event) { logger.log(event); }
};