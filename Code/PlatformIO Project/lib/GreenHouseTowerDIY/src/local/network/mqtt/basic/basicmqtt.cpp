#include "basicmqtt.hpp"

//***********************************************************************************************************************
// * Basic MQTT Stack
// * This is the basic MQTT stack that is used to connect to a local Mosquitto
// broker.
//************************************************************************************************************************

BaseMQTT::BaseMQTT(WiFiClient& espClient, GreenHouseConfig& config)
    : _espClient(espClient),
      _deviceConfig(config),
      PubSubClient(
          _deviceConfig.getBroker(),
          _deviceConfig.getMQTTConfig().port,
          [&](char* topic, byte* payload, unsigned int length) {
            mqttCallback(topic, payload, length);
          },
          espClient),
      networkConnected(false) {}

BaseMQTT::~BaseMQTT() {}

void BaseMQTT::begin() {
  log_i("[BasicMQTT]: Setting up MQTT...");

  //* Local Mosquitto Connection -- Start
  if (connect(DEFAULT_HOSTNAME)) {
    log_i("[BasicMQTT]: Connection succeeded. MQTT client state is: %d",
          state());
    networkConnected = true;
    return;
  }
  networkConnected = false;
  log_i("[BasicMQTT]: Connection failed. MQTT client state is: %d", state());
  //* Local Mosquitto Connection -- End
}

//* Handles messages arrived on subscribed topic(s)
void BaseMQTT::mqttCallback(char* topic, byte* payload, unsigned int length) {
  log_i("[BasicMQTT]: Message arrived on topic: [%s] ", topic);

  //* Convert the payload to a string
  std::string result;
  for (int i = 0; i < length; i++) {
    log_i("%s", static_cast<char>(payload[i]));
    result += static_cast<char>(payload[i]);
  }
  log_i("[BasicMQTT]: Message: [%s]", result.c_str());

  // TODO: Add support for all sensor topics
}

void BaseMQTT::visit(SensorInterface<float>* sensor) {
  std::string topic = sensor->getSensorName();
  if (!networkConnected && !topic.empty()) {
    this->subscribe(topic.c_str());
  }
  std::string payload = std::to_string(sensor->read());
  if (!topic.empty() && !payload.empty()) {
    publish(topic.c_str(), payload.c_str(), payload.length());
  }
}

/**
 * @brief Reconnect to the MQTT broker
 * @note This is a blocking function and will not return until the connection is
 * established
 */
void BaseMQTT::mqttReconnect() {
  //* Loop until we're reconnected
  while (!connected()) {
    log_i("[BasicMQTT]: Attempting MQTT connection...");
    //* Attempt to connect
    networkConnected = false;
    if (connect(DEFAULT_HOSTNAME)) {
      log_i("[BasicMQTT]: Connected to MQTT broker.");
      networkConnected = true;
      return;
    }
    log_e("[BasicMQTT]: Failed, rc= %d", state());
    log_i("[BasicMQTT]: Trying again in 15 seconds");
    //* Wait 15 seconds before retrying
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
void BaseMQTT::mqttLoop() {
  Network_Utilities::my_delay(1L);
  if (!connected()) {
    mqttReconnect();
  } else {
    loop();
  }
}
