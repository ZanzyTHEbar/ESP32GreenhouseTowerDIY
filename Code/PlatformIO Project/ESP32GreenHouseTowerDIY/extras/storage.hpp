#pragma once
#include <SPIFFS.h>
#include "ArduinoJson.h"
#include "observer.hpp"
#include <vector>
#include <string>

#define DESERIALIZE_CONFIG_SIZE 1024
#define SERIALIZE_CONFIG_SIZE 1024

struct DeviceConfig
{
    char name[32];
    char OTAPassword[64];
    int OTAPort = 0;
};

struct PumpConfig
{
    char name[32];
    int pin = 0;
    int onTime = 0;
    int offTime = 0;
};

struct WiFiConfig
{
    char name[7];
    char ssid[64];
    char password[64];
};

struct MQTTConfig
{
};

struct TowerConfig
{
    DeviceConfig device{};
    PumpConfig pump_config{};
    std::vector<WiFiConfig> networks;
    MQTTConfig mqtt_config{};
};

class Configuration : public ISubject
{
public:
    void setup(const char *fileName);
    void loadConfig();

    DeviceConfig *getDeviceConfig() { return &this->config.device; }
    PumpConfig *getPumpConfig() { return &this->config.pump_config; }
    std::vector<WiFiConfig> *getWifiConfigs() { return &this->config.networks; }
    MQTTConfig *getMQTTConfig() { return &this->config.mqtt_config; }

    void updateDeviceConfig(JsonObject &deviceConfig, bool shouldNotify);
    void updatePumpConfig(JsonObject &pumpConfig, bool shouldNotify);
    void updateNetwork(char *networkName, JsonObject &wifiConfig, bool shouldNotify);
    void updateMQTTConfig(JsonObject &mqttConfig, bool shouldNotify);
    void reset() { SPIFFS.format(); }
    void save();

private:
    char configFileName[20];
    TowerConfig config;
    bool already_loaded = false;
};