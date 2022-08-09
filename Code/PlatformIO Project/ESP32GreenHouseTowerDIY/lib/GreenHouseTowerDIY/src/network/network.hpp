/*
 Network.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
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

#include <defines.hpp>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "data/Config/config.hpp"
#include "data/StateManager/StateManager.hpp"

extern WiFiClient espClient;

class Network
{
public:
    // constructors
    Network();
    virtual ~Network();
    // Functions
    bool SetupNetworkStack();
    void SetupWebServer();
    void SetupServer();
    void networkRoutes();
    void CheckNetworkLoop();
    void CheckConnectionLoop_Active();
    bool LoopWifiScan();
    void setWiFiConf(const char *ssid, const char *password);

    // variables
private:
    int CheckWifiState();
    // Timer variables
    unsigned long _previousMillis;
    const long _interval; // interval to wait for Wi-Fi connection (milliseconds)
    bool _wifiConnected;
};

extern Network network;
#endif
