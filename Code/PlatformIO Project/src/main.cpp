#include <Arduino.h>

// Utilities
#include <local/logo/logo.hpp>
#include <utilities/network_utilities.hpp>
// Config
#include <data/StateManager/StateManager.hpp>
#include <data/config/project_config.hpp>

// Network
#include <local/network/api/api.hpp>
#include <network/mDNS/MDNSManager.hpp>
#include <network/ota/OTA.hpp>
// Data
#include <local/data/AccumulateData/accumulatedata.hpp>
#include <local/data/config/config.hpp>

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
#include "local/data/BackgroundTasks/taskManager.hpp"

// IO
#include "local/io/Relays/relays.hpp"

// Objects
GreenHouseConfig configManager("greenhouse");
APIServer server(80, &configManager, "/control", "/wifimanager",
                 "/userCommands");

WiFiHandler network(&configManager, &wifiStateManager, WIFI_SSID, WIFI_PASS, 1);
OTA ota(&configManager);
MDNSHandler mDNS(&mdnsStateManager, &configManager,
                 ("_" + configManager.getHostname()), "data", "_tcp",
                 "api_port", "80");
TaskManager timedTasks(&configManager);
API api(&wifiStateManager, &server, &configManager, &timedTasks);
NetworkNTP ntp;
TowerTemp tower_temp;
Humidity humidity;
WaterLevelSensor waterLevelSensor(&tower_temp);
AccumulateData data(&configManager, &ntp, &tower_temp, &humidity,
                    &waterLevelSensor);

I2C_RelayBoard relays(&configManager);

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(DEBUG_MODE);
    Logo::printASCII();

    /* Setup Events and Background Tasks */
    configManager.attach(&mDNS); // attach the mDNS to the config manager
    timedTasks.setTask(ObserverEvent::CustomEvents::relaysActivated,
                       &relays); // attach the relays to the timed tasks
    timedTasks.setTask(ObserverEvent::CustomEvents::accumulateData,
                       &data); // attach the data to the timed tasks

    /* Load Config from memory */
    configManager.initConfig(); // call before load to initialise the structs
    configManager.load();       // load the config from flash

    network.begin();
    mDNS.startMDNS();
    api.begin();
    ntp.begin();
    ota.SetupOTA();
    humidity.begin();
    tower_temp.begin();
    waterLevelSensor.begin();
}

void loop()
{
    Network_Utilities::checkWiFiState(); // check the WiFi state
    ota.HandleOTAUpdate();               // handle OTA updates
    data.loop();                         // accumulate sensor data
    timedTasks.taskHandler();            // handle background tasks
}
