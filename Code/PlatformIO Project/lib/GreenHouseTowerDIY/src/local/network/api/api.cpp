#include "api.hpp"

API::API(StateManager<WiFiState_e> *stateManager, APIServer *server,
         GreenHouseConfig *configManager)
    : stateManager(stateManager), configManager(configManager), server(server) {}

API::~API()
{
    delete stateManager;
    delete configManager;
    delete server;
}

void API::printHelloWorld() { Serial.println("Hello World"); }

void API::begin()
{
    // handle the WiFi connection state changes
    switch (stateManager->getCurrentState())
    {
    case WiFiState_e::WiFiState_Disconnected:
    {
        break;
    }
    case WiFiState_e::WiFiState_Disconnecting:
    {
        break;
    }
    case WiFiState_e::WiFiState_ADHOC:
    {
        server->begin();
        log_d("[SETUP]: Starting API Server");
        break;
    }
    case WiFiState_e::WiFiState_Connected:
    {
        server->updateCommandHandlers(
            "helloWorld", [&](AsyncWebServerRequest *request)
            { printHelloWorld(); });
        server->begin();
        log_d("[SETUP]: Starting API Server");
        break;
    }
    case WiFiState_e::WiFiState_Connecting:
    {
        break;
    }
    case WiFiState_e::WiFiState_Error:
    {
        break;
    }
    }
}
