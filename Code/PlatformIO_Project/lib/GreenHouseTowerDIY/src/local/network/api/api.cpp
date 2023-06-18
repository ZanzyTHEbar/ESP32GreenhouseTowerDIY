#include "api.hpp"

API::API(APIServer& server, GreenHouseConfig& configManager)
    : configManager(configManager), server(server) {}

API::~API() {}

void API::setupServer() {
  /* server.updateCommandHandlers("helloWorld",
                               [&](AsyncWebServerRequest* request) {
                                 printHelloWorld();
                                 request->send(200, "text/plain", "OK");
                               });
  server.updateCommandHandlers(
      "addRelay", [&](AsyncWebServerRequest* request) { addRelay(request); });
  server.updateCommandHandlers(
      "removeRelay",
      [&](AsyncWebServerRequest* request) { removeRelay(request); }); */
  server.begin();
}

void API::begin() {
  // handle the WiFi connection state changes
  switch (wifiStateManager.getCurrentState()) {
    case WiFiState_e::WiFiState_Disconnected: {
      break;
    }
    case WiFiState_e::WiFiState_Disconnecting: {
      break;
    }
    case WiFiState_e::WiFiState_ADHOC: {
      setupServer();
      log_d("[SETUP]: Starting API Server");
      break;
    }
    case WiFiState_e::WiFiState_Connected: {
      setupServer();
      log_d("[SETUP]: Starting API Server");
      break;
    }
    case WiFiState_e::WiFiState_Connecting: {
      break;
    }
    case WiFiState_e::WiFiState_Error: {
      break;
    }
  }
}

void API::setDHT(AsyncWebServerRequest* request) {
  switch (server._networkMethodsMap_enum[request->method()]) {
    case APIServer::POST: {
      std::string type;
      uint8_t pin;
      int params = request->params();
      log_d("Number of Params: %d", params);
      for (int i = 0; i < params; i++) {
        AsyncWebParameter* param = request->getParam(i);
        log_i("%s[%s]: %s\n",
              server._networkMethodsMap[request->method()].c_str(),
              param->name().c_str(), param->value().c_str());
        if (param->name() == "type") {
          type.assign(param->value().c_str());
        } else if (param->name() == "pin") {
          pin = atoi(param->value().c_str());
        }
      }
      if (type.empty() || pin == 0) {
        request->send(400, APIServer::MIMETYPE_JSON,
                      "{\"msg\":\"Invalid Request - please provide values for "
                      "all parameters\"}");
        Network_Utilities::my_delay(0.5L);
        request->redirect("/");
        return;
      }
    }
    default: {
      request->send(400, APIServer::MIMETYPE_JSON,
                    "{\"msg\":\"Invalid Request\"}");
      request->redirect("/");
      break;
    }
  }
}
