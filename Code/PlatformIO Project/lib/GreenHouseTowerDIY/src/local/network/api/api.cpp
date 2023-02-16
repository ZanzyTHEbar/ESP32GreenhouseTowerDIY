#include "api.hpp"

API::API(StateManager<WiFiState_e> *stateManager, APIServer *server,
         GreenHouseConfig *configManager)
    : stateManager(stateManager), configManager(configManager), server(server)
{
}

API::~API() {}

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
    // only start the API server if we have wifi connection
    // server->updateCommandHandlers("blink", blink);                // add a
    // command handler to the API server - you can add as many as you want -
    // you can also add methods. server->updateCommandHandlers("helloWorld",
    // printHelloWorld); // add a command handler to the API server - you can
    // add as many as you want - you can also add methods.
    server->begin();
    log_d("[SETUP]: Starting API Server");
    break;
  }
  case WiFiState_e::WiFiState_Connected:
  {
    // only start the API server if we have wifi connection
    // server->updateCommandHandlers("blink", blink);                // add a
    // command handler to the API server - you can add as many as you want -
    // you can also add methods.
    server->updateCommandHandlers(
        "helloWorld", [&](AsyncWebServerRequest *request)
        { printHelloWorld(); }); // add a command handler to the API server - you
                                 // can add as many as you want - you can also add
                                 // methods.
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
