
// TODO: Implement observer for humidity sensor
// TODO: Implement pressure sensor for water level
// TODO: Implement IR sensor for water level
// FIXME: Fix CI build

#include <Arduino.h>

//* Utilities
#include <local/logo/logo.hpp>
#include <utilities/network_utilities.hpp>

//* Network
#include <local/network/api/api.hpp>
#include <network/mDNS/MDNSManager.hpp>
#include <network/ota/OTA.hpp>
#include "local/network/mqtt/basic/basicmqtt.hpp"
#include "local/network/mqtt/mdns/mdns.hpp"

//* Data
#include <local/data/accumulatedata/accumulatedata.hpp>
#include <local/data/config/config.hpp>

//*  Sensor Includes
#include <local/io/sensors/humidity/humidity.hpp>
#include <local/io/sensors/light/ldr.hpp>
#include <local/io/sensors/temperature/towertemp.hpp>
#include <local/io/sensors/water_level/waterlevelsensor.hpp>

//* Time stamp
#include <local/network/ntp/ntp.hpp>

//! Objects

//* Config
ProjectConfig config("greenhouse", "tower");
ConfigHandler configHandler(config);
GreenHouseConfig greenhouseConfig(config);

//* Network

WiFiClient espClient;

APIServer server(80, config, "/control", "/wifimanager", "/tower");
WiFiHandler network(config, WIFI_SSID, WIFI_PASS, 1);
OTA ota(config);
MDNSHandler mDNS(config, "_tower", "data", "_tcp", "api_port", "80");
NetworkNTP ntp;
BaseMQTT mqtt(espClient, greenhouseConfig);

//* API
API api(server, greenhouseConfig);

//* Sensors
TowerTemp tower_temp;
Humidity humidity(greenhouseConfig);
WaterLevelSensor waterLevelSensor(tower_temp);
LDR ldr(greenhouseConfig);

AccumulateData data(ldr,
                    tower_temp,
                    humidity,
                    waterLevelSensor,
                    ntp,
                    config,
                    greenhouseConfig,
                    mqtt);

void setup() {
  Serial.begin(115200);
  Logo::printASCII();

  //* Setup Events and Background Tasks
  config.attach(configHandler);
  config.registerUserConfig(&greenhouseConfig);
  config.attach(network);
  config.attach(mDNS);  // attach the mDNS to the config manager

  //* Load Config from memory
  configHandler.begin();

  //* Setup Sensors
  humidity.begin();
  tower_temp.begin();

  //* Setup Network Tasks
  network.begin();
  mDNS.begin();
  //* Discover mDNS Broker
  mDNSDiscovery::discovermDNSBroker(config, greenhouseConfig);
  //* Setup MQTT
  mqtt.begin();
  api.begin();
  ntp.begin();
  ota.begin();
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
  mqtt.mqttLoop();                      // handle MQTT
  ota.handleOTAUpdate();                // handle OTA updates
  data.loop();                          // accumulate sensor data
}
