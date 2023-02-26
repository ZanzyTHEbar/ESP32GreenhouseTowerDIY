#include <Arduino.h>

//* Utilities
#include <local/logo/logo.hpp>
#include <utilities/network_utilities.hpp>

//* Network
#include <local/network/api/api.hpp>
#include <network/mDNS/MDNSManager.hpp>
#include <network/ota/OTA.hpp>

//* Data
#include <local/data/AccumulateData/accumulatedata.hpp>
#include <local/data/config/config.hpp>

//*  Sensor Includes
#include <local/io/sensors/humidity/Humidity.hpp>
#include <local/io/sensors/light/bh1750.hpp>
#include <local/io/sensors/light/ldr.hpp>
#include <local/io/sensors/pH/pHsensor.hpp>
#include <local/io/sensors/temperature/towertemp.hpp>
#include <local/io/sensors/water_level/waterlevelsensor.hpp>

//* IO Includes
#include <local/io/Relays/relays.hpp>

//* Time stamp
#include <local/network/ntp/ntp.hpp>

//* Background tasks
#include "local/data/BackgroundTasks/taskManager.hpp"

//* Objects

//* Config
GreenHouseConfig configManager("greenhouse");

//* Network
APIServer server(80,
                 &configManager,
                 "/control",
                 "/wifimanager",
                 "/userCommands");

WiFiHandler network(&configManager, &wifiStateManager, WIFI_SSID, WIFI_PASS, 1);
OTA ota(&configManager);
MDNSHandler mDNS(&mdnsStateManager,
                 &configManager,
                 ("_esp32tower"),
                 "data",
                 "_tcp",
                 "api_port",
                 "80");
NetworkNTP ntp;

//* Tasks
TaskManager timedTasks(&configManager);

//* API
API api(&wifiStateManager, &server, &configManager, &timedTasks);

//* Sensors
TowerTemp tower_temp;
Humidity humidity(&configManager);
WaterLevelSensor waterLevelSensor(&tower_temp);
BH1750 bh1750;
LDR ldr;
PHSENSOR phsensor;

AccumulateData data(&phsensor,
                    &bh1750,
                    &ldr,
                    &tower_temp,
                    &humidity,
                    &waterLevelSensor,
                    &ntp,
                    &relays,
                    &configManager);

I2C_RelayBoard relays(&configManager);

// TODO: Add Serial Manager to handle enabled features
// TODO: Change macros to enums to enable or disable features

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(DEBUG_MODE);
  Logo::printASCII();

  //* Setup Events and Background Tasks
  configManager.attach(&mDNS);  // attach the mDNS to the config manager
  timedTasks.setTask(ObserverEvent::CustomEvents::relaysActivated,
                     &relays);  // attach the relays to the timed tasks
  timedTasks.setTask(ObserverEvent::CustomEvents::accumulateData,
                     &data);  // attach the data to the timed tasks

  //* Load Config from memory
  configManager.initConfig();  // call before load to initialise the structs
  configManager.load();        // load the config from flash

  //* Setup Network
  network.begin();
  mDNS.startMDNS();
  api.begin();
  ntp.begin();
  ota.SetupOTA();

  //* Setup Sensors
  humidity.begin();
  tower_temp.begin();
  waterLevelSensor.begin();
}

/**
 * @brief Main Loop
 * The main loop is responsible for handling in the following order:
 * 1. WiFi State
 * 2. OTA Updates
 * 3. Accumulate Data
 * 4. Background Tasks
 */
void loop() {
  Network_Utilities::checkWiFiState();  // check the WiFi state
  ota.HandleOTAUpdate();                // handle OTA updates
  data.loop();                          // accumulate sensor data
  timedTasks.taskHandler();             // handle background tasks
}
