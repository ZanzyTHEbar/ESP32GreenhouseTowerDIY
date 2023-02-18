#include "api.hpp"

API::API(StateManager<WiFiState_e> *stateManager, APIServer *server,
         GreenHouseConfig *configManager, TaskManager *taskManager)
    : stateManager(stateManager), configManager(configManager), taskManager(taskManager), server(server) {}

API::~API()
{
    delete stateManager;
    delete configManager;
    delete server;
}

void API::printHelloWorld() { Serial.println("Hello World"); }

void API::setupServer()
{
    server->updateCommandHandlers(
        "helloWorld", [&](AsyncWebServerRequest *request)
        { printHelloWorld();
            request->send(200, "text/plain", "OK"); });
    server->updateCommandHandlers(
        "addRelay", [&](AsyncWebServerRequest *request)
        { addRelay(request); });
    server->begin();
}

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
        setupServer();
        log_d("[SETUP]: Starting API Server");
        break;
    }
    case WiFiState_e::WiFiState_Connected:
    {
        setupServer();
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

void API::addRelay(AsyncWebServerRequest *request)
{
    switch (server->_networkMethodsMap_enum[request->method()])
    {
    case APIServer::POST:
    {
        std::string name;
        uint8_t port = 0;
        bool start_state = false;
        timeObj *timer = nullptr;

        int params = request->params();
        log_d("Number of Params: %d", params);
        for (int i = 0; i < params; i++)
        {
            AsyncWebParameter *param = request->getParam(i);
            if (param->name() == "name")
            {
                name.assign(param->value().c_str());
            }
            else if (param->name() == "port")
            {
                port = atoi(param->value().c_str());
            }
            else if (param->name() == "state")
            {
                start_state = atoi(param->value().c_str());
            }
            else if (param->name() == "timer")
            {
                timer = new timeObj(atoi(param->value().c_str()));
            }

            log_i("%s[%s]: %s\n",
                  server->_networkMethodsMap[request->method()].c_str(),
                  param->name().c_str(), param->value().c_str());
        }

        if (name.empty() || port == 0 || timer == nullptr)
        {
            request->send(400, APIServer::MIMETYPE_JSON, "{\"msg\":\"Invalid Request - please provide values for all parameters\"}");
            Network_Utilities::my_delay(0.5L);
            request->redirect("/");
            return;
        }
        taskManager->setRelaysConfig(name, port, start_state, timer);
        request->send(200, APIServer::MIMETYPE_JSON, "{\"msg\":\"Successfully added a relay device\"}");
        break;
    }
    default:
    {
        request->send(400, APIServer::MIMETYPE_JSON, "{\"msg\":\"Invalid Request\"}");
        request->redirect("/");
        break;
    }
    }
}