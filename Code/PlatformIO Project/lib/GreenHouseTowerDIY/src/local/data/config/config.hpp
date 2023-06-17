#ifndef GREENHOUSE_CONFIG_HPP
#define GREENHOUSE_CONFIG_HPP
#include <Arduino.h>
#include <timeObj.h>
#include <data/config/project_config.hpp>
#include <unordered_map>

namespace Project_Config {
  struct EnabledFeatures_t {
    enum DHT_Features_e : uint8_t {
      DHT11 = (unsigned char)11U,
      DHT22 = (unsigned char)22U,
      DHT21 = (unsigned char)21U,
    };
    enum Humidity_Features_e {
      NONE_HUMIDITY,
      SHT31,
      SHT31_2,
      BOTH_HUMIDITY,
      DHT,
      DHT_SHT31,
      DHT_SHT31_2,
    };
    enum LDR_Features_e : uint8_t {
      NONE_LDR,
      LDR,
      BH1750,
      // BH1750_FAST,
      // BH1750_GND,
      // BH1750_VCC,
      ALL_LDR
    };
    enum Temp_Features_e : uint8_t { NONE_TEMP, TEMP_F, TEMP_C };
    enum Water_Level_Features_e : uint8_t {
      NONE_WATER_LEVEL,
      WATER_LEVEL_UC,
      WATER_LEVEL_PRESSURE,
      WATER_LEVEL_IR,
      ALL_WATER_LEVEL
    };

    enum Mqtt_Secure_e : uint8_t { SECURE_MQTT, INSECURE_MQTT };

    Humidity_Features_e humidity_features;
    DHT_Features_e dht_features;
    Temp_Features_e temp_features;
    LDR_Features_e ldr_features;
    Water_Level_Features_e water_Level_features;

    std::string dht_type;
    uint8_t dht_pin;
  };

  struct MQTTConfig_t {
    bool enabled;
    bool reconnect_mqtt;
    int reconnect_tries;
    int reconnect_time_ms;
    std::string broker;
    uint16_t port;
    Project_Config::EnabledFeatures_t::Mqtt_Secure_e auth;
    std::string username;
    std::string password;
    bool enable_certs;
    std::string ca_file;
    std::string cert_file;
    std::string key_file;
    bool enabled_websocket;
    std::string websocket_path;
    std::vector<std::string> pub_topics;
    std::vector<std::string> sub_topics;
    int mqtt_task_stack_size;
  };

  class GreenHouseConfig_t : ProjectConfig_t {
   protected:
    MQTTConfig_t mqtt;
    EnabledFeatures_t enabled_features;
  };
}  // namespace Project_Config

class GreenHouseConfig : public CustomConfigInterface,
                         private Project_Config::GreenHouseConfig_t {
  ProjectConfig& projectConfig;

 public:
  GreenHouseConfig(ProjectConfig& projectConfig);
  ~GreenHouseConfig();

  //* CustomConfigInterface
  void load() override;
  void save() override;

  //* Load
  void loadMQTT();
  void loadFeatures();

  //* Save
  void saveMQTT();
  void saveFeatures();
  void initConfig();

  std::string toRepresentation();

  void setMQTTConfig(const std::string& broker,
                     const std::string& username = std::string(),
                     const std::string& password = std::string(),
                     uint16_t port = 1883);

  void setMQTTBroker(const std::string& broker, uint16_t port = 1883);

  Project_Config::MQTTConfig_t& getMQTTConfig();
  Project_Config::EnabledFeatures_t& getEnabledFeatures();

  IPAddress getBroker();

  /* Types */
  typedef Project_Config::EnabledFeatures_t::Humidity_Features_e
      HumidityFeatures_t;
  typedef Project_Config::EnabledFeatures_t::DHT_Features_e DHTFeatures_t;
  typedef Project_Config::EnabledFeatures_t::LDR_Features_e LDRFeatures_t;
  typedef Project_Config::EnabledFeatures_t::Temp_Features_e TempFeatures_t;
  typedef Project_Config::EnabledFeatures_t::Water_Level_Features_e
      WaterLevelFeatures_t;
  typedef Project_Config::EnabledFeatures_t::Mqtt_Secure_e MqttSecure_t;
};

#endif
