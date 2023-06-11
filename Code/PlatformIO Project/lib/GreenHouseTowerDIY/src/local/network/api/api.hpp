#ifndef API_HPP
#define API_HPP
#include <EasyNetworkManager.hpp>
#include <data/statemanager/StateManager.hpp>
#include <functional>
#include <local/data/BackgroundTasks/taskManager.hpp>
#include <local/data/config/config.hpp>
class API {
 private:
  APIServer& server;
  GreenHouseConfig& configManager;
  TaskManager& taskManager;
  void setupServer();

 public:
  API(APIServer& server,
      GreenHouseConfig& configManager,
      TaskManager& taskManager);
  virtual ~API();
  void begin();
  void printHelloWorld();
  void addRelay(AsyncWebServerRequest* request);
  void removeRelay(AsyncWebServerRequest* request);
  void setDHT(AsyncWebServerRequest* request);
};

#endif  // API_HPP