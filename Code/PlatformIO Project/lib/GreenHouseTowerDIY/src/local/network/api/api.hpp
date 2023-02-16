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

public:
  API(StateManager<WiFiState_e> *stateManager,
      APIServer *server,
      GreenHouseConfig *configManager,
      TaskManager *taskManager);
  virtual ~API();
  void begin();
  void printHelloWorld();
  void addRelay(AsyncWebServerRequest *request);
};

#endif // API_HPP