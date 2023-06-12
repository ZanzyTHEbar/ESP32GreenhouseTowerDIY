#include "basicmqtt.hpp"

//***********************************************************************************************************************
// * Basic MQTT Stack
// * This is the basic MQTT stack that is used to connect to a local Mosquitto
// broker.
//************************************************************************************************************************

BASEMQTT::BASEMQTT(WiFiClient& espClient,
                   AccumulateData& accumulateData,
                   GreenHouseConfig& config)
    : _espClient(espClient),
      _accumulateData(accumulateData),
      _deviceConfig(config),
      PubSubClient(
          _deviceConfig.getBroker(),
          _deviceConfig.getMQTTConfig().port,
          [&](char* topic, byte* payload, unsigned int length) {
            mqttCallback(topic, payload, length);
          },
          espClient),
      _interval(60000),
      _previousMillis(0) {}

BASEMQTT::~BASEMQTT() {}

void BASEMQTT::begin() {
  log_i("[BasicMQTT]: Setting up MQTT...");

  //* Local Mosquitto Connection -- Start
  if (connect(DEFAULT_HOSTNAME)) {
    log_i("[BasicMQTT]: Connection succeeded. MQTT client state is: %d",
          state());
    subscribeAll();
    return;
  }
  log_i("[BasicMQTT]: Connection failed. MQTT client state is: %d", state());
  //* Local Mosquitto Connection -- End
}

//* Handles messages arrived on subscribed topic(s)
void BASEMQTT::mqttCallback(char* topic, byte* payload, unsigned int length) {
  log_i("[BasicMQTT]: Message arrived on topic: [%s] ", topic);

  //* Convert the payload to a string
  std::string result;
  for (int i = 0; i < length; i++) {
    log_i("%s", static_cast<char>(payload[i]));
    result += static_cast<char>(payload[i]);
  }
  log_i("[BasicMQTT]: Message: [%s]", result.c_str());

  // TODO: Add support for all sensor topics

  //* LDR

  //* TowerTemp

  //* Humidity

  //* WaterLevelSensor
}

void BASEMQTT::subscribeAll() {
  // Subscribe to all relay topics
  // TODO: Add ECC support based on return value of subscribe()

  // TODO: Add support for subscribing to all sensor topics

  //* BH1750

  //* LDR

  //* TowerTemp

  //* Humidity

  //* WaterLevelSensor
  // subscribe(relay.name.c_str());
  log_i("[BasicMQTT]: Successfully subscribed to all topics.");
}

void BASEMQTT::publishAll() {
  // TODO: Publish all sensor data

  //* LDR

  //* TowerTemp

  //* Humidity

  //* WaterLevelSensor

  // publish(topic, data);
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
    log_i("[BasicMQTT]: Attempting MQTT connection...");
    // Attempt to connect
    if (connect(DEFAULT_HOSTNAME)) {
      log_i("[BasicMQTT]: Connected to MQTT broker.");
      // Subscribe
      subscribeAll();
      return;
    }
    log_e("[BasicMQTT]: Failed, rc= %d", state());
    log_i("[BasicMQTT]: Trying again in 15 seconds");
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
