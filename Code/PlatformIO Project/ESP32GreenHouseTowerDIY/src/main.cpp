#include <defines.hpp>
// data Struct
#include "data/Config/config.hpp"
#include "data/StateManager/StateManager.hpp"
#include "network/mDNSManager/mDNSManager.hpp"
#include "io/LEDManager/LEDManager.hpp"
#include "io/SerialManager/serialhandler.hpp"

// IO
#include <Wire.h>
#if ENABLE_I2C_SCANNER
#include "io/i2cScanner/i2cscan.hpp"
#endif // ENABLE_I2C_SCANNER

// Light Sensors
#if USE_BH1750
#include "sensors/light/bh1750.hpp"
#else
#include "sensors/light/ldr.hpp"
#endif // USE_BH1750

// Temp Sensors
#include "sensors/temperature/towertemp.hpp"

// Humidity Sensors
#include "sensors/humidity/Humidity.hpp"

// Water Level Sensors
#include "sensors/water_level/waterlevelsensor.hpp"

// PH Sensors
#if ENABLE_PH_SUPPORT
#include "sensors/pH/pHsensor.hpp"
#endif // ENABLE_PH_SUPPORT

// Network definitions
#include "network/network.hpp"
#include "network/ntp.hpp"
#include "network/OTA/OTA.hpp"

// Accumulate Data
#include "data/AccumulateData/accumulatedata.hpp"

// Timed tasks
#include "data/BackgroundTasks/timedtasks.hpp"

/*######################## MQTT Configuration ########################*/
// MQTT includes
#if ENABLE_MDNS_SUPPORT
#include "network/mDNSManager/mDNSManager.hpp"
#endif // ENABLE_MDNS_SUPPORT
#if ENABLE_HASS
#include "mqtt/HASSIO/hassmqtt.hpp"
#else
#include "mqtt/BASIC/basicmqtt.hpp"
#endif // ENABLE_HASS
/*###################### MQTT Configuration END ######################*/

// Relays and other IO
#include "io/Relays/Relays.hpp"
#include "io/Pump/pump.hpp"

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

mDNSManager::MDNSHandler mdnsHandler(&StateManager_MDNS, &cfg);

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  timedTasks.begin();
  ledManager.begin();

  Serial.println(F("Setting up the program, standby..."));
  // Setup the main loop

  if (Wire.begin())
  {
    Serial.println(F("I2C initialized"));
  }
  else
  {
    Serial.println(F("I2C failed"));
  }

  Serial.println(F("Green House Tower booting - please wait"));
  Serial.println(F("Starting..."));
  tower_temp.SetupSensors();
#if USE_UC
  waterlevel.begin();
#endif // USE_UC

  switch (humidity.setupSensor())
  {
  case 0:
    Serial.println(F("Humidity Sensor Setup Failed - No sensors present"));
    break;
  case 1:
    Serial.println(F("Humidity Sensor Setup Failed - initialised sensor one"));
    break;
  case 2:
    Serial.println(F("Humidity Sensor Setup Failed - initialised sensor two"));
    break;
  case 3:
    Serial.println(F("Humidity Sensor Setup Successful"));
    break;
  default:
    Serial.println(F("Humidity Sensor Setup Failed - Unknown Error"));
    break;
  }

  Serial.println("");
  // Relay.SetupPID();
  // Setup the network stack
  Serial.println(F("Setting up WiFi"));
  Serial.println(F("Starting Webserver"));
  network.SetupWebServer();
  cfg.attach(&mdnsHandler);
  network.SetupServer();
  Serial.println(F("Setting up MQTT"));
#if ENABLE_HASS
  hassmqtt.loadMQTTConfig();
#else
  basemqtt.loadMQTTConfig();
#endif // ENABLE_HASS

#if ENABLE_MDNS_SUPPORT
  mDNSManager::mdnsHandler.startMDNS();
  if (mDNSManager::mdnsHandler.DiscovermDNSBroker())
  {
    Serial.println(F("[mDNS responder started] Setting up Broker..."));
  }
  else
  {
    Serial.println(F("[mDNS responder failed]"));
  }
#endif // ENABLE_MDNS_SUPPORT

#if ENABLE_HASS
  hassmqtt.begin();
#else
  basemqtt.begin();
#endif // ENABLE_HASS
  if (network.SetupNetworkStack())
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
  ledManager.indicateWaterLevel(waterlevelSensor.getWaterLevel());
  serialHandler.loop();

#if ENABLE_I2C_SCANNER
  timedTasks.ScanI2CBus();
#endif // ENABLE_I2C_SCANNER
  timedTasks.accumulateSensorData();
  timedTasks.updateCurrentData();

  if (cfg.config.data_json)
  {
    cfg.config.data_json = false;
    if (accumulatedata.SendData())
    {
      Serial.println(F("Data Sent"));
    }
    else
    {
      Serial.println(F("Data Not Sent"));
    }
  }

  if (StateManager_WiFi.getCurrentState() == ProgramStates::DeviceStates::WiFiState_e::WiFiState_Connected)
  {
    timedTasks.NTPService();
#if ENABLE_HASS
    hassmqtt.mqttLoop();
#else
    basemqtt.mqttLoop();
#endif // ENABLE_HASS
  }
  else
  {
    Serial.println("MQTT is disabled until a wifi connection has been established.");
  }
}
