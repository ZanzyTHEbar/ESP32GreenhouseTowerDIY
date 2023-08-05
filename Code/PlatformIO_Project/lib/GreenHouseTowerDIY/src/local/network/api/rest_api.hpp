#ifndef API_HPP
#define API_HPP
#include <EasyNetworkManager.hpp>
#include <data/statemanager/state_manager.hpp>
#include <local/data/config/config.hpp>
class RestAPI {
 private:
  APIServer server;
  ProjectConfig& projectConfig;
  GreenHouseConfig& configManager;
  void setupServer();

 public:
  RestAPI(ProjectConfig& projectConfig, GreenHouseConfig& configManager);
  virtual ~RestAPI();
  void begin();
  void setTopic(AsyncWebServerRequest* request);
  void setDHT(AsyncWebServerRequest* request);
};

#endif  // API_HPP