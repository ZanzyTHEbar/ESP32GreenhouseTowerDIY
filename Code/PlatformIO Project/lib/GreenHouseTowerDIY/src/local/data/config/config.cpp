#include "config.hpp"

GreenHouseConfig::GreenHouseConfig(const std::string& hostname)
    : hostname(hostname), ProjectConfig(std::string(), hostname) {}

GreenHouseConfig::~GreenHouseConfig() {}

void GreenHouseConfig::initConfig() {
  ProjectConfig::initConfig();
  this->config.mqtt = {"", 0, "", ""};
}

void GreenHouseConfig::load() {
  ProjectConfig::load();
  // TODO: Load relays
  // TODO: Load MQTT
}

void GreenHouseConfig::save() {
  ProjectConfig::save();
  // TODO: Save relays
  // TODO: Save MQTT
}

//**********************************************************************************************************************
//*
//!                                                ToRepresentation
//*
//**********************************************************************************************************************

std::string Project_Config::MQTTConfig_t::toRepresentation() {
  std::string json = Helpers::format_string(
      "\"deviceData\": {\"broker\": \"%s\", \"port\": \"%d\", \"username\": "
      "\"%s\", \"password\": \"%s\"}",
      this->broker.c_str(), this->port, this->username.c_str(),
      this->password.c_str());
  return json;
}

std::string Project_Config::RelaysConfig_t::toRepresentation() {
  std::string json = Helpers::format_string(
      "\"deviceData\": {\"name\": \"%s\", \"port\": \"%d\", \"start_state\": "
      "\"%s\", \"timer\": \"%.3f\"}",
      this->name.c_str(), this->port, this->start_state ? "on" : "off",
      this->timer->getTime());
  return json;
}

//**********************************************************************************************************************
//*
//!                                                GetMethods
//*
//**********************************************************************************************************************

Project_Config::MQTTConfig_t* GreenHouseConfig::getMQTTConfig() {
  return &this->config.mqtt;
}

std::vector<Project_Config::RelaysConfig_t>*
GreenHouseConfig::getRelaysConfig() {
  return &this->config.relays;
}

std::string GreenHouseConfig::getHostname() const {
  return this->hostname;
}
