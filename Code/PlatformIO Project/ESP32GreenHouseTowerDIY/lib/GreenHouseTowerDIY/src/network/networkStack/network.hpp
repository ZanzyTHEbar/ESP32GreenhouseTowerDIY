/*
 network.hpp - BUTT_TRACKER library
 Copyright (c) 2021 DaOfficialWizard.
*/

#pragma once
#ifndef NETWORK_hpp
#define NETWORK_hpp
extern "C"
{
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event.h>
}

#include <memory>
#include <string>
#include <functional>
#include <defines.hpp>
#include <WiFi.h>

#include "data/Config/config.hpp"
#include "data/StateManager/StateManager.hpp"
#include "utilities.hpp"

class Network
{
private:
    int CheckWifiState();
    // Timer variables
    unsigned long _previousMillis;
    const long _interval; // interval to wait for Wi-Fi connection (milliseconds)

    IPAddress localIP;
    StateManager<ProgramStates::DeviceStates::WiFiState_e> *stateManager;
    typedef std::function<void(void)> callback_t;
    callback_t mCallback;

public:
    Network(Config *cfg, StateManager<ProgramStates::DeviceStates::WiFiState_e> *stateManager);
    virtual ~Network();
    void begin();
    bool setup();
    void SetupServer();
    void CheckIfValidHostname();
    void CheckNetworkLoop();
    void CheckConnectionLoop_Active();
    void setWiFiConf(const char *value, uint8_t *location, wifi_config_t *conf);

    void setCallback(callback_t callback);
    std::shared_ptr<WiFiClient> espClient;
    std::unique_ptr<wifi_config_t> wifi_conf;
    Config *cfg;
};
#endif
// End of file