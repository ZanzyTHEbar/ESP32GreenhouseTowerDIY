#ifndef API_HPP
#define API_HPP
#include <EasyNetworkManager.hpp>
#include <data/statemanager/StateManager.hpp>
#include <functional>
#include <local/data/config/config.hpp>
class API {
private:
  StateManager<WiFiState_e> *stateManager;
  GreenHouseConfig *configManager;
  APIServer *server;

public:
  API(StateManager<WiFiState_e> *stateManager, APIServer *server,
      GreenHouseConfig *configManager);
  virtual ~API();
  void begin();
  void printHelloWorld();
};

#endif // API_HPP