/*
 Network.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
*/
#pragma once
#ifndef Network_hpp
#define Network_hpp

#include <defines.hpp>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

extern WiFiClient espClient;

class Network
{
public:
    // constructors
    Network();
    virtual ~Network();
    // Functions
    bool SetupNetworkStack();
    bool connectToApWithFailToStation();
    void SetupWebServer();
    void SetupServer();
    void CheckNetworkLoop();
    void CheckConnectionLoop_Active();
    void SetupWifiScan();
    void networkRoutes();
    bool LoopWifiScan();

    // variables
private:
    int CheckWifiState();
    const size_t MAX_FILESIZE; // 2MB
    // Timer variables
    unsigned long previousMillis;
    const long interval; // interval to wait for Wi-Fi connection (milliseconds)
    bool wifiConnected;
};

extern Network network;
#endif
