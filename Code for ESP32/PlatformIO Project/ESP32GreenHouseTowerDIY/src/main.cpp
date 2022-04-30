#include <Arduino.h>
#include <defines.hpp>

/******************************************************************************
 * Function: Setup Main Loop
 * Description: This is the setup function for the main loop of the whole program. Use this to setup the main loop.
 * Parameters: None
 * Return: None
 ******************************************************************************/
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

  // use a c++ ranged for loop to iterate through the relay pins
  for (auto pin : cfg.config.relays_pin)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // C for loop - legacy function
  /* for (int i = 0; i < sizeof(cfg.config.relays_pin) / sizeof(cfg.config.relays_pin[0]); i++)
  {
      pinMode(cfg.config.relays_pin[i], OUTPUT);
      digitalWrite(cfg.config.relays_pin[i], LOW);
  } */

  Wire.begin();

  Serial.println(F("HMS booting - please wait"));
  Serial.println(F("Starting..."));
  Cell_Temp.SetupSensors();

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
  network.SetupWebServer();
  Serial.println(F("Starting Webserver"));
  network.SetupServer();
  Serial.println(F("Setting up WiFi"));
  Serial.println(F("Setting up MQTT"));
  HMSmqtt.loadMQTTConfig();

#if ENABLE_MDNS_SUPPORT
  if (ENABLE_MQTT_SUPPORT)
  {
    HMSmqtt.DiscovermDNSBroker(); // discover the mDNS broker for mqtt
  }
#endif // ENABLE_MDNS_SUPPORT

  HMSmqtt.MQTTSetup();

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
  timedTasks.ScanI2CBus();
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
    HMSmqtt.RunMqttService();
  }
  my_delay(1L);
}