/*
 Network.h - HBAT library
 Copyright (c) 2021 Zacariah Austin Heim.
*/
#pragma once
#ifndef HMSNETWORK_hpp
#define HMSNETWORK_hpp

#include "defines.hpp"
#include "SecureOTA.hpp"
#include <ESPmDNS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

extern AsyncWebServer server;
extern WiFiClient espClient;

class HMSnetwork
{
public:
    // constructors
    HMSnetwork();
    virtual ~HMSnetwork();
    // Functions
    bool SetupNetworkStack();
    bool connectToApWithFailToStation();
    void SetupWebServer();
    void SetupServer();
    void CheckNetworkLoop();
    void SetupWifiScan();
    void networkRoutes();
    bool LoopWifiScan();

    // variables
private:
    int CheckWifiState();
    int maxVoltage;
    int maxTemp;
};

extern HMSnetwork network;
#endif
