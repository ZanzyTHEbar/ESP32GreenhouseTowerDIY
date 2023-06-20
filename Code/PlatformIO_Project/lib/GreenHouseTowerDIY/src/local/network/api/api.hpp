#ifndef API_HPP
#define API_HPP
#include <EasyNetworkManager.hpp>
#include <data/statemanager/StateManager.hpp>
#include <functional>
#include <local/data/config/config.hpp>
class API {
 private:
  APIServer server;
  ProjectConfig& projectConfig;
  GreenHouseConfig& configManager;
  void setupServer();

 public:
  API(ProjectConfig& projectConfig, GreenHouseConfig& configManager);
  virtual ~API();
  void begin();
  void setTopic(AsyncWebServerRequest* request);
  void setDHT(AsyncWebServerRequest* request);
};

#endif  // API_HPP