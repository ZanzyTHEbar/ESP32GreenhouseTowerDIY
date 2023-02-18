#include "config.hpp"

GreenHouseConfig::GreenHouseConfig(const std::string &hostname)
    : hostname(hostname), ProjectConfig(std::string(), hostname) {}

GreenHouseConfig::~GreenHouseConfig() {}

void GreenHouseConfig::initConfig()
{
  ProjectConfig::initConfig();
  this->config.mqtt = {"", 0, "", ""};
}

//**********************************************************************************************************************
//*
//!                                                Load
//*
//**********************************************************************************************************************

void GreenHouseConfig::load()
{
  // TODO: Load MQTT

  loadRelays();
  ProjectConfig::load();
}

void GreenHouseConfig::loadRelays()
{
  int relays_count = getInt("relays_count", 0);

  //*! Note: The name must be less than 15 chars in size
  std::string name = "relay_";
  std::string port = "port_";
  std::string start_state = "start_state_";
  std::string timer = "timer_";

  for (int i = 0; i < relays_count; i++)
  {
    char buffer[2];

    std::string iter_str = Helpers::itoa(i, buffer, 10);

    name.append(iter_str);
    port.append(iter_str);
    start_state.append(iter_str);
    timer.append(iter_str);

    const std::string &name_temp = getString(name.c_str(), "").c_str();
    const uint8_t port_temp = getInt(port.c_str(), 0);
    const bool start_state_temp = getBool(start_state.c_str(), false);
    const float timer_temp = getFloat(timer.c_str(), 0.0f);

    //! We use emplace_back to avoid copying the object
    this->config.relays.emplace_back(name_temp, port_temp, start_state_temp,
                                     new timeObj(timer_temp));
  }
}

//**********************************************************************************************************************
//*
//!                                                Save
//*
//**********************************************************************************************************************

void GreenHouseConfig::save()
{
  ProjectConfig::save();
  saveRelays();
  // TODO: Save MQTT
}

void GreenHouseConfig::saveRelays()
{
  putInt("relays_count", this->config.relays.size());

  //*! Note: The name must be less than 15 chars in size
  std::string name = "relay_";
  std::string port = "port_";
  std::string start_state = "start_state_";
  std::string timer = "timer_";

  for (int i = 0; i < this->config.relays.size(); i++)
  {
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

//**********************************************************************************************************************
//*
//!                                                ToRepresentation
//*
//**********************************************************************************************************************

std::string Project_Config::MQTTConfig_t::toRepresentation()
{
  std::string json = Helpers::format_string(
      "\"deviceData\": {\"broker\": \"%s\", \"port\": \"%d\", \"username\": "
      "\"%s\", \"password\": \"%s\"}",
      this->broker.c_str(), this->port, this->username.c_str(),
      this->password.c_str());
  return json;
}

std::string Project_Config::RelaysConfig_t::toRepresentation()
{
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

Project_Config::MQTTConfig_t *GreenHouseConfig::getMQTTConfig()
{
  return &this->config.mqtt;
}

std::vector<Project_Config::RelaysConfig_t> *
GreenHouseConfig::getRelaysConfig()
{
  return &this->config.relays;
}

std::string GreenHouseConfig::getHostname() const { return this->hostname; }
