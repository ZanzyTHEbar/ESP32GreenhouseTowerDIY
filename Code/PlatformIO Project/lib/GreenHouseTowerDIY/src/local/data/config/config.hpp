#ifndef GREENHOUSE_CONFIG_HPP
#define GREENHOUSE_CONFIG_HPP
#include <Arduino.h>
#include <data/config/project_config.hpp>

namespace Project_Config {
  struct MQTTConfig_t {
    std::string broker;
    uint16_t port;
    std::string username;
    std::string password;
    std::string toRepresentation();
  };

  struct GreenHouseConfig_t : ProjectConfig_t {
    MQTTConfig_t mqtt;
  };
}  // namespace Project_Config

class GreenHouseConfig : public ProjectConfig {
 public:
  GreenHouseConfig(const std::string& hostname);
  ~GreenHouseConfig() override = default;

  void load() override;
  void save() override;
  void initConfig() override;

  std::string getHostname() const;

  Project_Config::MQTTConfig_t* getMQTTConfig();

  Project_Config::GreenHouseConfig_t config;

 private:
  std::string hostname;
};

#endif
