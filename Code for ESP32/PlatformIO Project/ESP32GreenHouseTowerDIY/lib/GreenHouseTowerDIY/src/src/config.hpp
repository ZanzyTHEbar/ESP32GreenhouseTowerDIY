/*
 config.hpp - HBAT library
 Copyright (c) 2021 Zacariah Austin Heim.
*/

#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP
#include "defines.hpp"

class Config
{
public:
    Config();
    virtual ~Config();
    struct Config_t
    {
        // Variables
        char *hostname;
        bool MQTTEnabled;
        char *MQTTClientID;
        int MQTTPort;        // Port to use for unsecured MQTT
        int MQTTPort_Secure; // port to use if Secured MQTT is enabled
        char *MQTTUser;
        char *MQTTPass;
        char *MQTTTopic;
        char *MQTTSetTopic;
        char *MQTTDeviceName;
        int last_mqtt_connect_attempt;
        int last_mqtt_publish_attempt;
        unsigned long lastMillis;
        char *IP;
        char *netmask;
        char *gateway;
        bool MQTTSecureState;
        char *MQTTBroker;
        long lastMsg;
        char *msg;
        int value;
        char *WIFISSID;
        char *WIFIPASS;
        bool MQTTConnectedState;
        char *NTPTIME;
        char *NTPTIMEOFFSET;
        char *MDNS;
        char *DHCPCHECK;
        bool data_json;
        String data_json_string;
        bool relays[5];
        int relays_pin[5];
        float stack_humidity;
        float stack_temp;
        float stack_voltage;
        float stack_current;
        float cell_temp[10];
        float cell_voltage[10];
        int numSensors;
        int cell_count_max;
        int flow_rate;
        int flow_rate_sensor_temp;
    };
    Config_t config;

    bool loadConfig();
    // trigger a config write/commit
    bool setConfigChanged();
    bool updateCurrentData();
    // overwrite all config settings with "0"
    void resetConfig();
    bool saveConfig();
    bool isValidHostname(char *hostname_to_check, long size);
    // parse and set a new hostname to config
    void setHostname(String new_hostname);
    // we can't assing wifiManager.resetSettings(); to reset, somehow it gets called straight away.
    void setWiFiConf(String ssid, String password);
    void InitDataStruct();
    void CreateDefaultConfig();
    bool initSPIFFS();
    String readFile(fs::FS &fs, const char *path);
    void writeFile(fs::FS &fs, const char *path, const char *message);

private:
    Config_t default_cfg;
    int last_config;
    // save last "timestamp" the config has been saved
    bool last_config_change;
    // Variables
    int maxVoltage;
    int maxTemp;
    String doc_string;
    // Temporary function to ensure that the correct number of cells are being read - this will be removed when the cell count is dynamically allocated
};

extern Config cfg;
#endif
// EOF