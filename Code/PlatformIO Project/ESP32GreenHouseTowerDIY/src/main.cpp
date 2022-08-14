#include <defines.hpp>
#include "io/SerialManager/serialhandler.hpp"
#include "io/LEDManager/LEDManager.hpp"
#include "data/BackgroundTasks/timedtasks.hpp"
#include "network/OTA/OTA.hpp"
#include "network/api/webServerHandler.hpp"

/*######################## MQTT Configuration ########################*/
// MQTT includes
#include "mqtt/mqttbase.hpp"
#if ENABLE_MDNS_SUPPORT
#include "network/mDNSManager/mDNSManager.hpp"
#endif // ENABLE_MDNS_SUPPORT
#if ENABLE_HASS
#include "mqtt/HASSIO/hassMqtt.hpp"
#else
#include "mqtt/Basic/basicmqtt.hpp"
#endif // ENABLE_HASS
/*###################### MQTT Configuration END ######################*/

//******************************************************************************
//* Function: Main program entry point.
//* Description: This is the entry point of the program.
//* Parameters: None
//* Return: None
// ******************************************************************************

#ifdef DEFAULT_HOSTNAME
#pragma message(Reminder DEFAULT_HOSTNAME " is defined as the default hostname.")
#endif

#if PRODUCTION
#pragma message(Reminder "This is a production build.")
#else
#pragma message(Reminder "This is a development build.")
#endif

SerialHandler serialHandler;
Config config;
APIServer apiServer;
Network network;
LDR ldr;
TowerTemp tower_temp;
Humidity humidity;
WaterLevelSensor waterLevelSensor;
PUMP pump;
Relays relays;
PHSENSOR phsensor;
AccumulateData accumulateData;
NetworkNTP ntp;

#if ENABLE_MDNS_SUPPORT
mDNSManager::MDNSHandler mdnsHandler(&StateManager_MDNS, &cfg);
#endif // ENABLE_MDNS_SUPPORT

XMqttBaseClass baseMQTT;
#if ENABLE_HASS
HASSMQTT hassMqtt(&network, &config, &pump, &relays, &accumulateData, &phsensor, &ntp, &baseMQTT, &humidity, &tower_temp, &ldr);
#else
BasicMqtt basicMqtt(&network, &config, &pump, &relays, &accumulateData, &phsensor, &ntp, &baseMQTT, &humidity, &tower_temp, &ldr);
#endif // ENABLE_HASS

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Green House Tower booting - please wait"));

#if PRODUCTION
#pragma message(Reminder "This is a production build.")
  Serial.setDebugOutput(true);
#else
#pragma message(Reminder "Debugging is disabled.")
  Serial.setDebugOutput(false);
#endif

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  timedTasks.begin();
  ledManager.begin();

  Serial.println(F("Beginning the Setup up of your GreenHouse Tower, standby..."));
  // Setup the main loop
  Serial.println(F(Wire.begin() ? "I2C initialized" : "I2C failed to initialize"));
  tower_temp.SetupSensors();
#if USE_UC
  waterLevelSensor.begin();
#endif // USE_UC

  humidity.setup();

  Serial.println("");
  // Relay.SetupPID();
  // Setup the network stack
  Serial.println(F("Setting up WiFi"));
  Serial.println(F("Starting Webserver"));
  network.setCallback([&](void) -> void
                      { apiServer.begin(); });
  network.begin();
#if ENABLE_MDNS_SUPPORT
  cfg.attach(&mdnsHandler);
#endif // ENABLE_MDNS_SUPPORT
  network.SetupServer();
  Serial.println(F("Setting up MQTT"));
  baseMQTT.loadMQTTConfig();

#if ENABLE_MDNS_SUPPORT
  mDNSManager::mdnsHandler.startMDNS();
  log_d(mdnsHandler.DiscovermDNSBroker() ? "[mDNS responder started] Setting up Broker..." : "[mDNS responder]: failed");
#endif // ENABLE_MDNS_SUPPORT

#if ENABLE_HASS
  hassMqtt.begin();
#else
  basicMqtt.begin();
#endif // ENABLE_HASS
  if (network.setup())
  {
    Serial.println(F("Network Stack Setup Successful"));
    Serial.println(F("INFO: HTTP web server started"));
  }
  else
  {
    Serial.println(F("Network Stack Setup Failed - Activating Access-Point Mode"));
  }
  pump.begin();
  serialHandler.begin();
  ota.SetupOTA();

  Serial.print(F("\n===================================\n"));
  Serial.println(F("Setup Complete"));
  my_delay(1L);
}

void loop()
{
  idle(); // Run the magic that happens in the background. (Buttons & blinking).
  timedTasks.checkNetwork();
  ota.HandleOTAUpdate();
  ledManager.displayStatus();
  ledManager.indicateWaterLevel(waterLevelSensor.getWaterLevel());
  serialHandler.loop();

#if ENABLE_I2C_SCANNER
  timedTasks.ScanI2CBus();
#endif // ENABLE_I2C_SCANNER
  timedTasks.accumulateSensorData();
  timedTasks.updateCurrentData();

  if (cfg.config.data_json)
  {
    cfg.config.data_json = false;
    if (!accumulateData.SendData())
    {
      Serial.println(F("Data Sent"));
      log_e("Data Not Sent");
    }
  }

  if (StateManager_WiFi.getCurrentState() == WiFiState_e::WiFiState_Connected)
  {
    timedTasks.NTPService();
#if ENABLE_HASS
    hassMqtt.mqttLoop();
#else
    basicMqtt.mqttLoop();
#endif // ENABLE_HASS
  }
  else
  {
    Serial.println("MQTT is disabled until a wifi connection has been established.");
  }
}
