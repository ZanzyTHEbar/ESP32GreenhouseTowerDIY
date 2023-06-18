#ifndef LDR_HPP
#define LDR_HPP
#include <Arduino.h>
#include <hp_BH1750.h>
#include <utilities/network_utilities.hpp>
#include "local/data/config/config.hpp"
#include "local/data/visitor.hpp"

#define LDR_PIN 33
class LDR : public Element<Visitor<SensorInterface<float>>>,
            public SensorInterface<float> {
 public:
  explicit LDR(GreenHouseConfig& config);
  virtual ~LDR();
  void begin();
  float read() override;
  const std::string& getSensorName() override;
  void accept(Visitor<SensorInterface<float>>& visitor) override;

 private:
  GreenHouseConfig& config;
  hp_BH1750 BH1750_sensor;  // create the sensor object
  const float _GAMMA;
  const float _RL10;
};
#endif
