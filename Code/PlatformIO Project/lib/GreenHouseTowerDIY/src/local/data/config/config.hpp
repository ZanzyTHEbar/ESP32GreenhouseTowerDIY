#ifndef GREENHOUSE_CONFIG_HPP
#define GREENHOUSE_CONFIG_HPP
#include <Arduino.h>
#include <device-types/HASwitch.h>
#include <timeObj.h>
#include <data/config/project_config.hpp>
#include <unordered_map>

namespace Project_Config {
  struct MQTTConfig_t {
    std::string broker;
    uint16_t port;
    std::string username;
    std::string password;
  };

  struct RelaysConfig_t {
    RelaysConfig_t(const std::string& name,
                   uint8_t port,
                   bool start_state,
                   timeObj* timer,
                   HASwitch* ha_switch)
        : name(std::move(name)),
          port(port),
          start_state(start_state),
          timer(timer),
          ha_switch(ha_switch) {}
    std::string name;
    uint8_t port;
    bool start_state;
    timeObj* timer;
    HASwitch* ha_switch;
  };

  struct EnabledFeatures_t {
    enum Humidity_Features_e {
      NONE_HUMIDITY,
      SHT31,
      SHT31x2,
      DHT11,
      DHT22,
      DHT21,
      ALL_HUMIDITY
    };
    enum LDR_Features_e {
      NONE_LDR,
      LDR,
      BH1750,
      BH1750_FAST,
      BH1750_GND,
      BH1750_VCC,
      ALL_LDR
    };
    enum Water_Level_Features_e {
      NONE_WATER_LEVEL,
      WATER_LEVEL_UC,
      WATER_LEVEL_PRESSURE,
      WATER_LEVEL_IR,
      ALL_WATER_LEVEL
    };

    Humidity_Features_e humidity_Features;
    LDR_Features_e ldr_Features;
    Water_Level_Features_e water_Level_Features;
    std::string dht_type;
    uint8_t dht_pin;
  };

  struct GreenHouseConfig_t : ProjectConfig_t {
    MQTTConfig_t mqtt;
    std::vector<RelaysConfig_t> relays;
    EnabledFeatures_t enabled_features;
  };
}  // namespace Project_Config

class GreenHouseConfig : public CustomConfigInterface {
  ProjectConfig& projectConfig;

 public:
  GreenHouseConfig(ProjectConfig& projectConfig);
  ~GreenHouseConfig();

  //* CustomConfigInterface
  void load() override;
  void save() override;

  //* Load
  void loadRelays();
  void loadMQTT();
  void loadFeatures();

  //* Save
  void saveRelays();
  void saveMQTT();
  void saveFeatures();
  void initConfig();

  std::string toRepresentation();

  Project_Config::MQTTConfig_t* getMQTTConfig();
  std::vector<Project_Config::RelaysConfig_t>* getRelaysConfig();
  Project_Config::RelaysConfig_t* getRelayConfig(int index);
  Project_Config::EnabledFeatures_t* getEnabledFeatures();

  IPAddress getBroker();

  Project_Config::GreenHouseConfig_t config;

  /* Types */
  typedef Project_Config::EnabledFeatures_t::Humidity_Features_e
      HumidityFeatures_t;
  typedef Project_Config::EnabledFeatures_t::LDR_Features_e LDRFeatures_t;
  typedef Project_Config::EnabledFeatures_t::Water_Level_Features_e
      WaterLevelFeatures_t;
};

#endif
