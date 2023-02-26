#include "config.hpp"

GreenHouseConfig::GreenHouseConfig(const std::string& hostname)
    : ProjectConfig(std::string(), hostname) {}

GreenHouseConfig::~GreenHouseConfig() {}

void GreenHouseConfig::initConfig() {
  ProjectConfig::initConfig();
  this->config.mqtt = {"", 0, "", ""};
}

//**********************************************************************************************************************
//*
//!                                                Load
//*
//**********************************************************************************************************************

void GreenHouseConfig::load() {
  loadRelays();
  loadMQTT();
  loadFeatures();
  ProjectConfig::load();
}

void GreenHouseConfig::loadRelays() {
  int relays_count = getInt("relays_count", 0);

  //*! Note: The name must be less than 15 chars in size
  std::string name = "relay_";
  std::string port = "port_";
  std::string start_state = "start_state_";
  std::string timer = "timer_";

  for (int i = 0; i < relays_count; i++) {
    char buffer[2];

    std::string iter_str = Helpers::itoa(i, buffer, 10);

    name.append(iter_str);
    port.append(iter_str);
    start_state.append(iter_str);
    timer.append(iter_str);

    const std::string& name_temp = getString(name.c_str(), "").c_str();
    const uint8_t port_temp = getInt(port.c_str(), 0);
    const bool start_state_temp = getBool(start_state.c_str(), false);
    const float timer_temp = getFloat(timer.c_str(), 0.0f);

    //! We use emplace_back to avoid copying the object
    this->config.relays.emplace_back(name_temp, port_temp, start_state_temp,
                                     new timeObj(timer_temp));
  }
}

void GreenHouseConfig::loadMQTT() {
  this->config.mqtt.broker.assign(getString("mqtt_broker").c_str());
  this->config.mqtt.port = getInt("mqtt_port", 1886);
  this->config.mqtt.username.assign(getString("mqtt_username", "").c_str());
  this->config.mqtt.password.assign(getString("mqtt_password", "").c_str());
}

void GreenHouseConfig::loadFeatures() {
  this->config.enabled_features.humidity_Features =
      (HumidityFeatures_t)getInt("humidity_features", 0);
  this->config.enabled_features.ldr_Features =
      (LDRFeatures_t)getInt("ldr_features", 0);
  this->config.enabled_features.water_Level_Features =
      (WaterLevelFeatures_t)getInt("water_level_features", 0);
  this->config.enabled_features.dht_type.assign(
      getString("dht_type", "").c_str());
  this->config.enabled_features.dht_pin = getInt("dht_pin");
}

//**********************************************************************************************************************
//*
//!                                                Save
//*
//**********************************************************************************************************************

void GreenHouseConfig::save() {
  ProjectConfig::save();
  saveRelays();
  saveMQTT();
  saveFeatures();
}

void GreenHouseConfig::saveRelays() {
  putInt("relays_count", this->config.relays.size());

  //*! Note: The name must be less than 15 chars in size
  std::string name = "relay_";
  std::string port = "port_";
  std::string start_state = "start_state_";
  std::string timer = "timer_";

  for (int i = 0; i < this->config.relays.size(); i++) {
    char buffer[2];

    std::string iter_str = Helpers::itoa(i, buffer, 10);

    name.append(iter_str);
    port.append(iter_str);
    start_state.append(iter_str);
    timer.append(iter_str);

    putString(name.c_str(), this->config.relays[i].name.c_str());
    putInt(port.c_str(), this->config.relays[i].port);
    putBool(start_state.c_str(), this->config.relays[i].start_state);
    putFloat(timer.c_str(), this->config.relays[i].timer->getTime());
  }
}

void GreenHouseConfig::saveMQTT() {
  putString("mqtt_broker", this->config.mqtt.broker.c_str());
  putInt("mqtt_port", this->config.mqtt.port);
  putString("mqtt_username", this->config.mqtt.username.c_str());
  putString("mqtt_password", this->config.mqtt.password.c_str());
}

void GreenHouseConfig::saveFeatures() {
  putInt("humidity_features", this->config.enabled_features.humidity_Features);
  putInt("ldr_features", this->config.enabled_features.ldr_Features);
  putInt("water_features", this->config.enabled_features.water_Level_Features);

  putInt("dht_pin", this->config.enabled_features.dht_pin);
  putString("dht_type", this->config.enabled_features.dht_type.c_str());
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
  switch (this->config.enabled_features.humidity_Features) {
    case HumidityFeatures_t::NONE_HUMIDITY:
      hum.assign("None");
      break;
    case HumidityFeatures_t::DHT11:
      hum.assign("DHT11");
      break;
    case HumidityFeatures_t::DHT22:
      hum.assign("DHT22");
      break;
    case HumidityFeatures_t::DHT21:
      hum.assign("DHT21");
      break;
    case HumidityFeatures_t::SHT31:
      hum.assign("SHT31");
      break;
    case HumidityFeatures_t::SHT31x2:
      hum.assign("SHT31x2");
      break;
    case HumidityFeatures_t::ALL_HUMIDITY:
      hum.assign("All");
      break;
    default:
      hum.assign("None");
      break;
  }

  switch (this->config.enabled_features.ldr_Features) {
    case LDRFeatures_t::NONE_LDR:
      ldr.assign("None");
      break;
    case LDRFeatures_t::LDR:
      ldr.assign("LDR");
      break;
    case LDRFeatures_t::BH1750:
      ldr.assign("BH1750");
      break;
    case LDRFeatures_t::BH1750_FAST:
      ldr.assign("BH1750");
      break;
    case LDRFeatures_t::BH1750_GND:
      ldr.assign("BH1750 GND mode");
      break;
    case LDRFeatures_t::BH1750_VCC:
      ldr.assign("BH1750 VCC mode");
      break;
    case LDRFeatures_t::ALL_LDR:
      ldr.assign("All");
      break;
    default:
      ldr.assign("None");
      break;
  }

  switch (this->config.enabled_features.water_Level_Features) {
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
      this->config.mqtt.broker.c_str(), this->config.mqtt.port,
      this->config.mqtt.username.c_str(), this->config.mqtt.password.c_str());

  //* Relays Section
  std::string relays_json;
  for (int i = 0; i < this->config.relays.size(); i++) {
    relays_json.append(Helpers::format_string(
        "\"relays_%d\": {\"name\": \"%s\", \"port\": \"%d\", \"start_state\": "
        "\"%s\", \"timer\": \"%.3f\"}",
        i, this->config.relays[i].name.c_str(), this->config.relays[i].port,
        this->config.relays[i].start_state ? "on" : "off",
        this->config.relays[i].timer->getTime()));
  }

  //* Return formatted json string
  return Helpers::format_string("{%s, %s, %s}", mqtt_json.c_str(),
                                features_json.c_str(), relays_json.c_str());
}

//**********************************************************************************************************************
//*
//!                                                GetMethods
//*
//**********************************************************************************************************************

Project_Config::MQTTConfig_t* GreenHouseConfig::getMQTTConfig() {
  return &this->config.mqtt;
}

/**
 * @brief Get the MQTT Broker IP object
 */
IPAddress GreenHouseConfig::getBroker() {
  IPAddress broker_ip;
  Project_Config::MQTTConfig_t* mqttConfig = getMQTTConfig();
  if (!mqttConfig->broker.empty()) {
    log_d("[mDNS responder started] Setting up Broker...");
    return broker_ip.fromString(mqttConfig->broker.c_str());
  }
  log_d("[mDNS responder failed]");
  return broker_ip.fromString(MQTT_BROKER);
}

std::vector<Project_Config::RelaysConfig_t>*
GreenHouseConfig::getRelaysConfig() {
  return &this->config.relays;
}

Project_Config::RelaysConfig_t* GreenHouseConfig::getRelayConfig(int index) {
  return &this->config.relays[index];
}

Project_Config::EnabledFeatures_t* GreenHouseConfig::getEnabledFeatures() {
  return &this->config.enabled_features;
}
