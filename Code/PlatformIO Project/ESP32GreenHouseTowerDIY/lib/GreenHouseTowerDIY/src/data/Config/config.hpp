/*
 config.hpp - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
*/

#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <defines.hpp>
#include <observer.hpp>
#include <ArduinoJson.h>
#include <SPIFFS.h>

class Config : public ISubject
{
public:
    Config();
    virtual ~Config();
    struct Config_t
    {
        // Variables
        char *hostname;
        char *MQTTClientID;
        int MQTTPort;        // Port to use for unsecured MQTT
        int MQTTPort_Secure; // port to use if Secured MQTT is enabled
        char *MQTTUser;
        char *MQTTPass;
        char *MQTTTopic;
        char *MQTTSetTopic;
        char *MQTTDeviceName;
        char *MQTTBroker;
        bool MQTTSecureState;
        bool MQTTConnectedState;
        int last_mqtt_connect_attempt;
        int last_mqtt_publish_attempt;
        unsigned long lastMillis;
        char *IP;
        char *netmask;
        char *gateway;
        long lastMsg;
        char *msg;
        int value;
        //char *WIFISSID;
        //char *WIFIPASS;
        char *MDNS;
        bool data_json;
        String data_json_string;
        bool relays[5];
        int relays_pin[5];
        int numTempSensors;
    };
    Config_t config;

    Config_t *getDeviceConfig() { return &this->config; }

    struct OTA_t
    {
        char name[32];
        char OTAPassword[64];
        int OTAPort = 0;
    };

    OTA_t ota_config;

    OTA_t *getOTAConfig() { return &this->ota_config; }

    /* Erase all data in SPIFFS - VERY DESTRUCTIVE */
    void
    reset()
    {
        SPIFFS.format();
    }

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

    // we can't assign wifiManager.resetSettings(); to reset, somehow it gets called straight away.
    void setWiFiConf(String ssid, String password);
    void CreateDefaultConfig();
    bool initSPIFFS();
    String readFile(fs::FS &fs, const char *path);
    void writeFile(fs::FS &fs, const char *path, const char *message);

    void printASCII(const char *fileName)
    {
        String asciiART = readFile(SPIFFS, fileName);
        Serial.write(asciiART.c_str());
    }

private:
    Config_t _default_cfg;
    int _last_config;
    // save last "timestamp" the config has been saved
    bool _last_config_change;
    // Variables
    int _maxTemp;
    String _doc_string;
    // Temporary function to ensure that the correct number of cells are being read - this will be removed when the cell count is dynamically allocated
};

extern Config cfg;
#endif
// EOF