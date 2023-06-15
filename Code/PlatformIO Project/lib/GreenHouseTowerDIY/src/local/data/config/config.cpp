#include "config.hpp"

GreenHouseConfig::GreenHouseConfig(ProjectConfig& projectConfig)
    : projectConfig(projectConfig) {}

GreenHouseConfig::~GreenHouseConfig() {}

void GreenHouseConfig::initConfig() {
  this->mqtt = {"", 0, "", ""};
}

//**********************************************************************************************************************
//*
//!                                                Load
//*
//**********************************************************************************************************************

void GreenHouseConfig::load() {
  loadMQTT();
  loadFeatures();
}

void GreenHouseConfig::loadMQTT() {
  this->mqtt.broker.assign(projectConfig.getString("mqtt_broker").c_str());
  this->mqtt.port = projectConfig.getInt("mqtt_port", 1886);
  this->mqtt.username.assign(
      projectConfig.getString("mqtt_username", "").c_str());
  this->mqtt.password.assign(
      projectConfig.getString("mqtt_password", "").c_str());
}

void GreenHouseConfig::loadFeatures() {
  this->enabled_features.humidity_features =
      (HumidityFeatures_t)projectConfig.getInt("humidity_features", 0);
  this->enabled_features.ldr_features =
      (LDRFeatures_t)projectConfig.getInt("ldr_features", 0);
  this->enabled_features.water_Level_features =
      (WaterLevelFeatures_t)projectConfig.getInt("water_level_features", 0);
  this->enabled_features.dht_type.assign(
      projectConfig.getString("dht_type", "").c_str());
  this->enabled_features.dht_pin = projectConfig.getInt("dht_pin");
}

//**********************************************************************************************************************
//*
//!                                                Save
//*
//**********************************************************************************************************************

void GreenHouseConfig::save() {
  saveMQTT();
  saveFeatures();
}

void GreenHouseConfig::saveMQTT() {
  projectConfig.putString("mqtt_broker", this->mqtt.broker.c_str());
  projectConfig.putInt("mqtt_port", this->mqtt.port);
  projectConfig.putString("mqtt_username", this->mqtt.username.c_str());
  projectConfig.putString("mqtt_password", this->mqtt.password.c_str());
}

void GreenHouseConfig::saveFeatures() {
  projectConfig.putInt("humidity_features",
                       this->enabled_features.humidity_features);
  projectConfig.putInt("ldr_features", this->enabled_features.ldr_features);
  projectConfig.putInt("water_features",
                       this->enabled_features.water_Level_features);

  projectConfig.putInt("dht_pin", this->enabled_features.dht_pin);
  projectConfig.putString("dht_type", this->enabled_features.dht_type.c_str());
}

//**********************************************************************************************************************
//*
//!                                                ToRepresentation
//*
//**********************************************************************************************************************

std::string GreenHouseConfig::toRepresentation() {
  //* Features Section
  std::string hum;
  std::string ldr;
  std::string water;

  // convert enums to strings
  switch (this->enabled_features.dht_features) {
    case DHTFeatures_t::DHT11:
      hum.assign("DHT11");
      break;
    case DHTFeatures_t::DHT22:
      hum.assign("DHT22");
      break;
    case DHTFeatures_t::DHT21:
      hum.assign("DHT21");
      break;
  }

  switch (this->enabled_features.humidity_features) {
    case HumidityFeatures_t::SHT31:
      hum.assign("SHT31");
      break;
    case HumidityFeatures_t::DHT:
      hum.assign("DHT");
      break;
    default:
      hum.assign("None");
      break;
  }

  switch (this->enabled_features.ldr_features) {
    case LDRFeatures_t::NONE_LDR:
      ldr.assign("None");
      break;
    case LDRFeatures_t::LDR:
      ldr.assign("LDR");
      break;
    case LDRFeatures_t::BH1750:
      ldr.assign("BH1750");
      break;
    case LDRFeatures_t::ALL_LDR:
      ldr.assign("All");
      break;
    default:
      ldr.assign("None");
      break;
  }

  switch (this->enabled_features.water_Level_features) {
    case WaterLevelFeatures_t::NONE_WATER_LEVEL:
      water.assign("None");
      break;
    case WaterLevelFeatures_t::WATER_LEVEL_UC:
      water.assign("Water Level UltraSonic");
      break;
    case WaterLevelFeatures_t::WATER_LEVEL_PRESSURE:
      water.assign("Water Level Pressure");
      break;
    case WaterLevelFeatures_t::WATER_LEVEL_IR:
      water.assign("Water Level IR");
      break;
    case WaterLevelFeatures_t::ALL_WATER_LEVEL:
      water.assign("All");
      break;
    default:
      water.assign("None");
      break;
  }

  std::string features_json = Helpers::format_string(
      "\"features\": {\"humidity\": \"%s\", \"ldr\": \"%s\", \"water_level\": "
      "\"%s\"}",
      hum.c_str(), ldr.c_str(), water.c_str());

  //* MQTT Section
  std::string mqtt_json = Helpers::format_string(
      "\"deviceData\": {\"broker\": \"%s\", \"port\": \"%d\", \"username\": "
      "\"%s\", \"password\": \"%s\"}",
      this->mqtt.broker.c_str(), this->mqtt.port, this->mqtt.username.c_str(),
      this->mqtt.password.c_str());

  //* Return formatted json string
  return Helpers::format_string("{%s, %s, %s}", mqtt_json.c_str(),
                                features_json.c_str());
}

void GreenHouseConfig::setMQTTConfig(const std::string& broker,
                                     const std::string& username,
                                     const std::string& password,
                                     uint16_t port) {
  this->mqtt.username.assign(username);
  this->mqtt.password.assign(password);
  this->mqtt.port = port;
  this->mqtt.broker.assign(broker);
}

void GreenHouseConfig::setMQTTBroker(const std::string& broker, uint16_t port) {
  this->mqtt.port = port;
  this->mqtt.broker.assign(broker);
}

//**********************************************************************************************************************
//*
//!                                                GetMethods
//*
//**********************************************************************************************************************

Project_Config::MQTTConfig_t& GreenHouseConfig::getMQTTConfig() {
  return this->mqtt;
}

/**
 * @brief Get the MQTT Broker IP object
 */
IPAddress GreenHouseConfig::getBroker() {
  IPAddress broker_ip;
  Project_Config::MQTTConfig_t& mqttConfig = getMQTTConfig();
  log_d("[mDNS responder started]: Setting up Broker...");
  return broker_ip.fromString(mqttConfig.broker.c_str());
}

Project_Config::EnabledFeatures_t& GreenHouseConfig::getEnabledFeatures() {
  return this->enabled_features;
}
