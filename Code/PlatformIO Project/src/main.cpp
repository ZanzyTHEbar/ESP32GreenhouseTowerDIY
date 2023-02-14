#include <Arduino.h>

// Utilities
#include <local/logo/logo.hpp>
#include <utilities/network_utilities.hpp>
// Config
#include <data/StateManager/StateManager.hpp>
#include <data/config/project_config.hpp>

// Network
#include <EasyNetworkManager.hpp>
#include <network/mDNS/MDNSManager.hpp>
#include <network/ota/OTA.hpp>
// Data
#include <local/data/AccumulateData/accumulatedata.hpp>

// Sensors
// Temp local/io/Sensors
#include "local/io/sensors/temperature/towertemp.hpp"
// Humidity local/io/Sensors
#include "local/io/sensors/humidity/Humidity.hpp"
// Water Level local/io/Sensors
#include "local/io/sensors/water_level/waterlevelsensor.hpp"
// Time stamp
#include "local/network/ntp/ntp.hpp"
// Background tasks
#include "local/data/BackgroundTasks/timedtasks.hpp"

std::string hostname = "waterchamber";

// Objects
ProjectConfig configManager(std::string(), hostname);
WiFiHandler network(&configManager, &wifiStateManager, WIFI_SSID, WIFI_PASS, 1);

APIServer server(80,
                 &configManager,
                 "/control",
                 "/wifimanager",
                 "/userCommands");
OTA ota(&configManager);
MDNSHandler mDNS(&mdnsStateManager,
                 &configManager,
                 ("_" + hostname),
                 "data",
                 "_tcp",
                 "api_port",
                 "80");

NetworkNTP ntp;
TowerTemp tower_temp;
Humidity humidity;
WaterLevelSensor waterLevelSensor(&tower_temp);
AccumulateData data(&configManager,
                    &ntp,
                    &tower_temp,
                    &humidity,
                    &waterLevelSensor);
TimedTasks timedTasks(&data);

void printHelloWorld() {
  Serial.println("Hello World");
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(DEBUG_MODE);
  Logo::printASCII();
  configManager.attach(&mDNS);
  configManager.initConfig();  // call before load to initialise the structs
  configManager.load();        // load the config from flash

  network.begin();
  mDNS.startMDNS();

  // handle the WiFi connection state changes
  switch (wifiStateManager.getCurrentState()) {
    case WiFiState_e::WiFiState_Disconnected: {
      break;
    }
    case WiFiState_e::WiFiState_Disconnecting: {
      break;
    }
    case WiFiState_e::WiFiState_ADHOC: {
      // only start the API server if we have wifi connection
      // server.updateCommandHandlers("blink", blink);                // add a
      // command handler to the API server - you can add as many as you want -
      // you can also add methods. server.updateCommandHandlers("helloWorld",
      // printHelloWorld); // add a command handler to the API server - you can
      // add as many as you want - you can also add methods.
      server.begin();
      log_d("[SETUP]: Starting API Server");
      break;
    }
    case WiFiState_e::WiFiState_Connected: {
      // only start the API server if we have wifi connection
      // server.updateCommandHandlers("blink", blink);                // add a
      // command handler to the API server - you can add as many as you want -
      // you can also add methods.
      server.updateCommandHandlers(
          "helloWorld",
          printHelloWorld);  // add a command handler to the API server - you
                             // can add as many as you want - you can also add
                             // methods.
      server.begin();
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
  ntp.begin();
  ota.SetupOTA();
  humidity.begin();
  tower_temp.begin();
  waterLevelSensor.begin();
}

void loop() {
  Network_Utilities::checkWiFiState();
  ota.HandleOTAUpdate();
  data.loop();
  timedTasks.accumulateSensorData();
}
