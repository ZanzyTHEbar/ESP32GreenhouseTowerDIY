#include "basicmqtt.hpp"

//***********************************************************************************************************************
// * Class Global Variables
// * Please only make changes to the following class variables within the ini
// file. Do not change them here.
//************************************************************************************************************************

BASEMQTT::BASEMQTT(WiFiClient* espClient,
                   PHSENSOR* phsensor,
                   BH1750* bh1750,
                   LDR* ldr,
                   TowerTemp* towertemp,
                   Humidity* humidity,
                   WaterLevelSensor* waterlevelsensor,
                   I2C_RelayBoard* relayboard,
                   IPAddress* broker_ip,
                   GreenHouseConfig* config)
    : phsensor(phsensor),
      bh1750(bh1750),
      ldr(ldr),
      towertemp(towertemp),
      humidity(humidity),
      waterlevelsensor(waterlevelsensor),
      relayboard(relayboard),
      deviceConfig(config),
#if MQTT_SECURE
      PubSubClient(
          *broker_ip,
          MQTT_SECURE_PORT,
          [&](char* topic, byte* payload, unsigned int length) {
            log_i("MQTT Callback");
            mqttCallback(topic, payload, length);
          },
          *espClient),
#else
      PubSubClient(
          *broker_ip,
          MQTT_PORT,
          [&](char* topic, byte* payload, unsigned int length) {
            log_i("MQTT Callback");
            mqttCallback(topic, payload, length);
          },
          *espClient),
#endif  // MQTT_SECURE
      _interval(60000),
      _previousMillis(0) {
}

BASEMQTT::~BASEMQTT() {
  // Destructor
}

void BASEMQTT::subscribeAll() {
  // Subscribe to all relay topics
  // TODO: Add ECC support based on return value of subscribe()
  auto relayConfig = deviceConfig->getRelaysConfig();
  for (auto relay : *relayConfig) {
    log_i("Subscribing to the topic [%s]", relay.name.c_str());
    subscribe(relay.name.c_str());
  }

  // Subscribe to all sensor topics
#if ENABLE_PH_SUPPORT
  // connection succeeded
  log_i("Connection succeeded. Subscribing to the topic [%s]",
        phsensor->_pHTopic.c_str());
  subscribe(phsensor->_pHTopic.c_str());
#endif  // ENABLE_PH_SUPPORT

  // TODO: Add support for subscribing to all sensor topics

  log_i("Successfully subscribed to all topics.");
}

void BASEMQTT::begin() {
  log_i("Setting up MQTT...");

  // Local Mosquitto Connection -- Start
  if (connect(DEFAULT_HOSTNAME)) {
    log_i("Connection succeeded. MQTT client state is: %d", state());
    subscribeAll();
    return;
  }
  log_i("Connection failed. MQTT client state is: %d", state());
  // Local Mosquitto Connection -- End
}

// Handles messages arrived on subscribed topic(s)
void BASEMQTT::mqttCallback(char* topic, byte* payload, unsigned int length) {
  log_i("Message arrived on topic: [%s] ", topic);

  // Convert the payload to a string
  std::string result;
  for (int i = 0; i < length; i++) {
    log_i("%s", static_cast<char>(payload[i]));
    result += static_cast<char>(payload[i]);
  }
  log_i("Message: [%s]", result.c_str());

  // Check if the message is for a relay
  auto relayConfig = deviceConfig->getRelaysConfig();
  for (auto relay : *relayConfig) {
    if (relay.name == topic) {
      log_i("Setting relay [%s] to: [%s]", relay.name.c_str(), result.c_str());
      relayboard->eventListener(result, relay.port);
      return;
    }
  }

  // Check if the message is for the PH sensor
#if ENABLE_PH_SUPPORT
  if (phsensor->_pHTopic == topic) {
    log_i("Setting pH level to: [%s]", result.c_str());
    phsensor->eventListener(result);
  }
#endif  // ENABLE_PH_SUPPORT
}

void BASEMQTT::publishAll() {
  // Publish all sensor data
}

/**
 * @brief Reconnect to the MQTT broker
 * @note This is a blocking function and will not return until the connection is
 * established
 *
 */
void BASEMQTT::mqttReconnect() {
  // Loop until we're reconnected
  while (!connected()) {
    log_i("Attempting MQTT connection...");
    // Attempt to connect
    if (connect(DEFAULT_HOSTNAME)) {
      log_i("Connected to MQTT broker.");
      // Subscribe
      subscribeAll();
      return;
    }
    log_i("failed, rc= %d", state());
    log_i(" try again in 15 seconds");
    // Wait 15 seconds before retrying
    Network_Utilities::my_delay(15L);
  }
}

/**
 * @brief MQTT loop function
 * @note This function should be called in the main loop
 * @note This function will handle the reconnection to the MQTT broker
 * @note This function will handle the publishing of sensor data
 *
 */
void BASEMQTT::mqttLoop() {
  Network_Utilities::my_delay(1L);
  if (!connected()) {
    mqttReconnect();
  } else {
    loop();
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= _interval) {
      _previousMillis = currentMillis;
      // Publish all sensor data
      publishAll();
    }
  }
}
