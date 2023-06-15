#include "basicmqtt.hpp"
#include <ESPmDNS.h>

//***********************************************************************************************************************
// * Basic MQTT Stack
// * This is the basic MQTT stack that is used to connect to a local Mosquitto
// broker.
//************************************************************************************************************************

BaseMQTT::BaseMQTT(WiFiClient& espClient,
                   GreenHouseConfig& config,
                   ProjectConfig& projectConfig)
    : _espClient(espClient),
      _deviceConfig(config),
      _projectConfig(projectConfig),
      _client(espClient),
      networkConnected(false),
      brokerDiscovery(false) {}

BaseMQTT::~BaseMQTT() {}

void BaseMQTT::begin() {
  log_i("[BasicMQTT]: Setting up MQTT...");

  brokerDiscovery = discovermDNSBroker();

  log_i("[BasicMQTT]: Broker %s:%d",
        _deviceConfig.getMQTTConfig().broker.c_str(),
        _deviceConfig.getMQTTConfig().port);

  _client.setServer(_deviceConfig.getBroker(),
                    _deviceConfig.getMQTTConfig().port);
  _client.setCallback([&](char* topic, byte* payload, unsigned int length) {
    mqttCallback(topic, payload, length);
  });

  std::string hostname = _projectConfig.getMDNSConfig().hostname;
  log_i("[BasicMQTT]: Hostname: %s", hostname.c_str());

  //* Local Mosquitto Connection -- Start
  if (!_client.connect(hostname.c_str())) {
    networkConnected = false;
    log_e("[BasicMQTT]: Connection failed. MQTT client state is: %d",
          _client.state());
    //* Local Mosquitto Connection -- End
    return;
  }
  log_i("[BasicMQTT]: Connection succeeded. MQTT client state is: %d",
        _client.state());
  networkConnected = true;
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

void BaseMQTT::dataHandler(const std::string& topic,
                           const std::string& payload) {
  if (!networkConnected && !topic.empty()) {
    _client.subscribe(topic.c_str());
  }

  if (!topic.empty() && !payload.empty()) {
    _client.publish(topic.c_str(), payload.c_str(), payload.length());
  }
}

void BaseMQTT::dataHandler(const std::string& topic, float payload) {
  if (!networkConnected && !topic.empty()) {
    _client.subscribe(topic.c_str());
  }

  std::string payloadStr = std::to_string(payload);
  if (!topic.empty() && !payloadStr.empty()) {
    _client.publish(topic.c_str(), payloadStr.c_str(), payloadStr.length());
  }
}

void BaseMQTT::dataHandler(const std::string& topic,
                           std::vector<float> payload) {
  if (!networkConnected && !topic.empty()) {
    _client.subscribe(topic.c_str());
  }

  std::string payloadStr;
  for (auto& i : payload) {
    payloadStr += std::to_string(i) + ",";
  }
  if (!topic.empty() && !payloadStr.empty()) {
    _client.publish(topic.c_str(), payloadStr.c_str(), payloadStr.length());
  }
}

void BaseMQTT::dataHandler(const std::string& topic,
                           std::vector<std::string> payload) {
  if (!networkConnected && !topic.empty()) {
    _client.subscribe(topic.c_str());
  }

  std::string payloadStr;
  for (auto& i : payload) {
    payloadStr += i + ",";
  }
  if (!topic.empty() && !payloadStr.empty()) {
    _client.publish(topic.c_str(), payloadStr.c_str(), payloadStr.length());
  }
}

void BaseMQTT::dataHandler(const std::string& topic,
                           std::unordered_map<std::string, float> payload) {
  if (!networkConnected && !topic.empty()) {
    _client.subscribe(topic.c_str());
  }

  std::string payloadStr;
  for (auto& i : payload) {
    payloadStr += i.first + ":" + std::to_string(i.second) + ",";
  }
  if (!topic.empty() && !payloadStr.empty()) {
    _client.publish(topic.c_str(), payloadStr.c_str(), payloadStr.length());
  }
}

/**
 * @brief Reconnect to the MQTT broker
 * @note This is a blocking function and will not return until the connection is
 * established
 */
void BaseMQTT::mqttReconnect() {
  //* Loop until we're reconnected
  while (!_client.connected()) {
    log_i("[BasicMQTT]: Attempting MQTT connection...");
    //* Attempt to connect
    networkConnected = false;
    if (_client.connect(DEFAULT_HOSTNAME)) {
      log_i("[BasicMQTT]: Connected to MQTT broker.");
      networkConnected = true;
      return;
    }
    log_e("[BasicMQTT]: Failed, rc= %d", _client.state());
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
 */
void BaseMQTT::mqttLoop() {
  if (brokerDiscovery) {
    Network_Utilities::my_delay(1L);
    if (!_client.connected()) {
      mqttReconnect();
    }
    loop();
    return;
  }
  log_e("[MQTT]: Failed to connect to MQTT broker");
}

//******************************************************************************
// * Function: Discover mDNS Broker
// * Description: Find the mDNS broker and and port return them
// * Parameters: None
// * Return: int - 1 if success, 0 if fail
//******************************************************************************
bool BaseMQTT::discovermDNSBroker() {
  IPAddress mqttServer;
  // check if there is a WiFi connection
  if (wifiStateManager.getCurrentState() == WiFiState_e::WiFiState_Connected) {
    log_i("[mDNS Broker Discovery]: Connected to Wifi, proceeding... \n");
    log_i("[mDNS Broker Discovery]: Setting up mDNS... ");
    log_i("[mDNS Broker Discovery]: Querying MQTT broker service...");

    int n = MDNS.queryService("mqtt", "tcp");
    if (n != 0) {
      int mqttPort;
      // Found one or more MQTT services - use the first one.
      mqttServer = MDNS.IP(0);
      mqttPort = MDNS.port(0);
      _deviceConfig.setMQTTBroker(mqttServer.toString().c_str(), mqttPort);
      _deviceConfig.saveMQTT();
      log_i("[mDNS Broker Discovery]: MQTT broker found at: %s:%d",
            mqttServer.toString().c_str(), mqttPort);
      return true;
    }
    log_e("[Fail]: No mqtt service found on your network\n");
  }
  return false;
}