#pragma once
#include <WiFi.h>
#include <storage.hpp>
#include <StateManager.hpp>

namespace WiFiHandler
{
    void setupWifi(StateManager *stateManager, Configuration *trackerConfig);
}