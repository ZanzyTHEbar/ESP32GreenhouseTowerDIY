#ifndef API_HPP
#define API_HPP
#include <EasyNetworkManager.hpp>
#include <data/statemanager/StateManager.hpp>
#include <functional>
#include <local/data/config/config.hpp>
#include <local/data/BackgroundTasks/taskManager.hpp>
class API
{
private:
  StateManager<WiFiState_e> *stateManager;
  APIServer *server;
  GreenHouseConfig *configManager;
  TaskManager *taskManager;
  void setupServer();
  void handleDHTType(uint8_t type);

public:
  API(StateManager<WiFiState_e> *stateManager,
      APIServer *server,
      GreenHouseConfig *configManager,
      TaskManager *taskManager);
  virtual ~API();
  void begin();
  void printHelloWorld();
  void addRelay(AsyncWebServerRequest *request);
  void removeRelay(AsyncWebServerRequest *request);
  void setDHT(AsyncWebServerRequest *request);
};

#endif // API_HPP