#include "config.hpp"

GreenHouseConfig::GreenHouseConfig(ProjectConfig& projectConfig)
    : projectConfig(projectConfig) {}

GreenHouseConfig::~GreenHouseConfig() {}

void GreenHouseConfig::initConfig() {
  this->mqtt = {
      .enabled = true,
      .reconnect_mqtt = true,
      .reconnect_tries = 10,
      .reconnect_time_ms = 10000,
      .broker = "",
      .port = 1883,
      .auth = MqttSecure_t::INSECURE_MQTT,
      .username = "",
      .password = "",
      .enable_certs = false,
      .ca_file = "certs/ca.crt",
      .cert_file = "certs/client.der",
      .key_file = "certs/private.der",
      .enabled_websocket = false,
      .websocket_path = "/",
      .pub_topics = {},
      .sub_topics = {},
      .mqtt_task_stack_size = 7168,
  };
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
  this->mqtt.broker.assign(projectConfig.getString("mqtt_broker", "").c_str());
  this->mqtt.port = projectConfig.getInt("mqtt_port");
  this->mqtt.username.assign(
      projectConfig.getString("mqtt_username", "").c_str());
  this->mqtt.password.assign(
      projectConfig.getString("mqtt_password", "").c_str());

  this->mqtt.enabled = projectConfig.getBool("mqtt_enabled");
  this->mqtt.reconnect_mqtt = projectConfig.getBool("rc_mqtt");
  this->mqtt.reconnect_tries = projectConfig.getInt("rc_tries");
  this->mqtt.reconnect_time_ms = projectConfig.getInt("rc_time_ms");
  this->mqtt.auth = (MqttSecure_t)projectConfig.getInt("mqtt_auth");
  this->mqtt.enable_certs = projectConfig.getBool("enable_certs");
  this->mqtt.ca_file.assign(projectConfig.getString("ca_file", "").c_str());
  this->mqtt.cert_file.assign(projectConfig.getString("cert_file", "").c_str());
  this->mqtt.key_file.assign(projectConfig.getString("key_file", "").c_str());
  this->mqtt.enabled_websocket = projectConfig.getBool("en_ws");
  this->mqtt.websocket_path.assign(
      projectConfig.getString("ws_path", "").c_str());
  this->mqtt.mqtt_task_stack_size = projectConfig.getInt("mqtt_size");

  // TODO: pub_topics and sub_topics - use for loops
}

void GreenHouseConfig::loadFeatures() {
  this->enabled_features.humidity_features =
      (HumidityFeatures_t)projectConfig.getInt("hum_feats", 0);
  this->enabled_features.ldr_features =
      (LDRFeatures_t)projectConfig.getInt("ldr_feats", 0);
  this->enabled_features.water_Level_features =
      (WaterLevelFeatures_t)projectConfig.getInt("wtr_lvl_feats", 0);
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

  projectConfig.putBool("mqtt_enabled", this->mqtt.enabled);
  projectConfig.putBool("rc_mqtt", this->mqtt.reconnect_mqtt);
  projectConfig.putInt("rc_tries", this->mqtt.reconnect_tries);
  projectConfig.putInt("rc_time_ms", this->mqtt.reconnect_time_ms);
  projectConfig.putInt("mqtt_auth", this->mqtt.auth);
  projectConfig.putBool("enable_certs", this->mqtt.enable_certs);
  projectConfig.putString("ca_file", this->mqtt.ca_file.c_str());
  projectConfig.putString("cert_file", this->mqtt.cert_file.c_str());
  projectConfig.putString("key_file", this->mqtt.key_file.c_str());
  projectConfig.putBool("en_ws", this->mqtt.enabled_websocket);
  projectConfig.putString("ws_path", this->mqtt.websocket_path.c_str());
  projectConfig.putInt("mqtt_size", this->mqtt.mqtt_task_stack_size);
  // TODO: pub_topics and sub_topics - use for loops
}

void GreenHouseConfig::saveFeatures() {
  projectConfig.putInt("hum_feats", this->enabled_features.humidity_features);
  projectConfig.putInt("ldr_feats", this->enabled_features.ldr_features);
  projectConfig.putInt("water_feats",
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
      "\"mqtt\": {\"broker\": \"%s\", \"port\": %d, \"username\": \"%s\", "
      "\"password\": \"%s\", \"enabled\": %s, \"reconnect_mqtt\": %s, "
      "\"reconnect_tries\": %d, \"reconnect_time_ms\": %d, \"auth\": %s, "
      "\"enable_certs\": %s, \"ca_file\": \"%s\", \"cert_file\": \"%s\", "
      "\"key_file\": \"%s\", \"enabled_websocket\": %s, \"websocket_path\": "
      "\"%s\", \"mqtt_task_stack_size\": %d}",
      this->mqtt.broker.c_str(), this->mqtt.port, this->mqtt.username.c_str(),
      this->mqtt.password.c_str(), this->mqtt.enabled ? "true" : "false",
      this->mqtt.reconnect_mqtt ? "true" : "false", this->mqtt.reconnect_tries,
      this->mqtt.reconnect_time_ms, this->mqtt.auth ? "true" : "false",
      this->mqtt.enable_certs ? "true" : "false", this->mqtt.ca_file.c_str(),
      this->mqtt.cert_file.c_str(), this->mqtt.key_file.c_str(),
      this->mqtt.enabled_websocket ? "true" : "false",
      this->mqtt.websocket_path.c_str(), this->mqtt.mqtt_task_stack_size);

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
