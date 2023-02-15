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

  struct RelaysConfig_t {
    RelaysConfig_t(const std::string& name, uint8_t port)
        : name(std::move(name)), port(port) {}
    std::string name;
    uint8_t port;
    std::string toRepresentation();
  };

  struct GreenHouseConfig_t : ProjectConfig_t {
    MQTTConfig_t mqtt;
    std::vector<RelaysConfig_t> relays;
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
  std::vector<Project_Config::RelaysConfig_t>* getRelaysConfig();

  Project_Config::GreenHouseConfig_t config;

 private:
  std::string hostname;
};

#endif
