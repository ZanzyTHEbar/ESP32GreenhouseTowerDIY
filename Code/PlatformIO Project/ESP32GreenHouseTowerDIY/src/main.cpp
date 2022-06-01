#include <Arduino.h>
#include <defines.hpp>

/******************************************************************************
 * Function: Setup Main Loop
 * Description: This is the setup function for the main loop of the whole program. Use this to setup the main loop.
 * Parameters: None
 * Return: None
 ******************************************************************************/

#ifdef DEFAULT_HOSTNAME
#pragma message(Reminder DEFAULT_HOSTNAME " is defined as the default hostname.")
#endif

#if PRODUCTION
#pragma message(Reminder "This is a production build.")
#else
#pragma message(Reminder "This is a development build.")
#endif

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  timedTasks.setupTimers();

  Serial.println(F("Setting up the program, standby..."));
  // Setup the main loop
  // Initialize the relay pins
  int temp[5] = {45, 38, 36, 35, 48};
  // initialize the Relay pins and set them to off state
  std::copy(temp, temp + sizeof(temp) / sizeof(temp[0]), cfg.config.relays_pin);

  Serial.println(F("Starting NTP Client"));
  networkntp.SetupNTP();
  Wire.begin();

  Serial.println(F("Green House Tower booting - please wait"));
  Serial.println(F("Starting..."));
  tower_temp.SetupSensors();

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
  Relay.SetupPID();
  // Setup the network stack
  // Setup the Wifi Manager
#if ENABLE_HASS
  hassmqtt.loadMQTTConfig();
#else
  basemqtt.loadMQTTConfig();
#endif // ENABLE_HASS
  network.SetupWebServer();
  Serial.println(F("Starting Webserver"));
  network.SetupServer();
  Serial.println(F("Setting up WiFi"));
  Serial.println(F("Setting up MQTT"));

#if ENABLE_MDNS_SUPPORT
  int mDNSDiscovery::DiscovermDNSBroker();
#endif // ENABLE_MDNS_SUPPORT

  Serial.println("");
  if (network.SetupNetworkStack())
  {
    Serial.println(F("Network Stack Setup Successful"));
    Serial.println(F("INFO: HTTP web server started"));
  }
  else
  {
    Serial.println(F("Network Stack Setup Failed - Activating Access-Point Mode"));
  }

  Serial.print(F("\n===================================\n"));
  Serial.println(F("Setup Complete"));
  my_delay(1L);
}

void loop()
{
#if ENABLE_I2C_SCANNER
  timedTasks.ScanI2CBus();
#endif // ENABLE_I2C_SCANNER
  timedTasks.accumulateSensorData();
  timedTasks.checkNetwork();
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

  if (WiFi.status() == WL_CONNECTED)
  {
    timedTasks.NTPService();
#if ENABLE_HASS
    hassmqtt.mqttLoop();
#else
    basemqtt.mqttLoop();
#endif // ENABLE_HASS
  }
}