#pragma once
#ifndef UTILITIES_hpp
#define UTILITIES_hpp
#include <Arduino.h>
#include "network/networkStack/network.hpp"
#include <unordered_map>
namespace Network_Utilities
{
    bool LoopWifiScan();
    void SetupWifiScan();
    int CheckWifiState();
    int getStrength(int points);
    String generateDeviceID();
}
#endif // !UTILITIES_hpp