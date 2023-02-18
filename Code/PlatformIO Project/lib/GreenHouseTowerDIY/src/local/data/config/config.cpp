#include "config.hpp"

GreenHouseConfig::GreenHouseConfig(const std::string &hostname)
    : ProjectConfig(std::string(), hostname) {}

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
  loadMQTT();
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

void GreenHouseConfig::loadMQTT()
{
  this->config.mqtt.broker.assign(getString("mqtt_broker").c_str());
  this->config.mqtt.port = getInt("mqtt_port", 1886);
  this->config.mqtt.username.assign(getString("mqtt_username", "").c_str());
  this->config.mqtt.password.assign(getString("mqtt_password", "").c_str());
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
  saveMQTT();
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

void GreenHouseConfig::saveMQTT()
{
  putString("mqtt_broker", this->config.mqtt.broker.c_str());
  putInt("mqtt_port", this->config.mqtt.port);
  putString("mqtt_username", this->config.mqtt.username.c_str());
  putString("mqtt_password", this->config.mqtt.password.c_str());
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

/**
 * @brief Get the MQTT Broker IP object
 */
IPAddress GreenHouseConfig::getBroker()
{
  IPAddress broker_ip;
  Project_Config::MQTTConfig_t *mqttConfig = getMQTTConfig();
  if (!mqttConfig->broker.empty())
  {
    log_d("[mDNS responder started] Setting up Broker...");
    return broker_ip.fromString(mqttConfig->broker.c_str());
  }
  log_d("[mDNS responder failed]");
  return broker_ip.fromString(MQTT_BROKER);
}

std::vector<Project_Config::RelaysConfig_t> *
GreenHouseConfig::getRelaysConfig()
{
  return &this->config.relays;
}

//**********************************************************************************************************************
//*
//!                                                Misc
//*
//**********************************************************************************************************************

bool GreenHouseConfig::isValidHostname(char *hostname_to_check, long size)
{
  for (int i = 0; i < size; i++)
  {
    if (hostname_to_check[i] == '-' || hostname_to_check[i] == '.')
      continue;
    else if (hostname_to_check[i] >= '0' && hostname_to_check[i] <= '9')
      continue;
    else if (hostname_to_check[i] >= 'A' && hostname_to_check[i] <= 'Z')
      continue;
    else if (hostname_to_check[i] >= 'a' && hostname_to_check[i] <= 'z')
      continue;
    else if (hostname_to_check[i] == 0 && i > 0)
      break;
    return false;
  }
  return true;
}