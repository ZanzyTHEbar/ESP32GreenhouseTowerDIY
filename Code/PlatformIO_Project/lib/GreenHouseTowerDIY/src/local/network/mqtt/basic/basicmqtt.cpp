#include "basicmqtt.hpp"
#include <ESPmDNS.h>

// TODO: Implement the MQTT Stack as a base class for all MQTT based sensors
// Note: reimplement this library with the current Mqtt base class https://github.com/dawidchyrzynski/arduino-home-assistant
// Note: https://dawidchyrzynski.github.io/arduino-home-assistant/documents/library/discovery.html

//***********************************************************************************************************************
// * Basic MQTT Stack
// * This is the basic MQTT stack that is used to connect to a local Mosquitto
// broker.
//************************************************************************************************************************
BaseMQTT::BaseMQTT(GreenHouseConfig& config,
                   ProjectConfig& projectConfig,
                   MQTTClient& client)
    : _deviceConfig(config),
      _projectConfig(projectConfig),
      _client(client),
      brokerDiscovery(false) {}

BaseMQTT::~BaseMQTT() {}

void BaseMQTT::begin() {
  log_i("[BasicMQTT]: Setting up MQTT...");
  _client.addCallback(this);

  brokerDiscovery = discovermDNSBroker();

  log_i("[BasicMQTT]: Broker %s:%d",
        _deviceConfig.getMQTTConfig().broker.c_str(),
        _deviceConfig.getMQTTConfig().port);

  if (!_deviceConfig.getMQTTConfig().broker.empty() || brokerDiscovery) {
    //* Generate the MQTT configuration
    //* Configuration for MQTT
    StaticJsonDocument<384> doc;
    JsonObject mqttConfig = doc.createNestedObject("mqtt");

    mqttConfig["enabled"] = _deviceConfig.getMQTTConfig().enabled;
    mqttConfig["reconnect_mqtt"] = _deviceConfig.getMQTTConfig().reconnect_mqtt;
    mqttConfig["reconnect_retires"] =
        _deviceConfig.getMQTTConfig().reconnect_tries;
    mqttConfig["reconnect_time_ms"] =
        _deviceConfig.getMQTTConfig().reconnect_time_ms;
    mqttConfig["server"] = _deviceConfig.getMQTTConfig().broker;
    mqttConfig["port"] = _deviceConfig.getMQTTConfig().port;
    mqttConfig["id_name"] = _projectConfig.getMDNSConfig().hostname;
    mqttConfig["auth"] = (bool)_deviceConfig.getMQTTConfig().auth;
    mqttConfig["username"] = _deviceConfig.getMQTTConfig().username;
    mqttConfig["password"] = _deviceConfig.getMQTTConfig().password;
    mqttConfig["enable_certs"] = _deviceConfig.getMQTTConfig().enable_certs;
    mqttConfig["ca_file"] = _deviceConfig.getMQTTConfig().ca_file;
    mqttConfig["cert_file"] = _deviceConfig.getMQTTConfig().cert_file;
    mqttConfig["key_file"] = _deviceConfig.getMQTTConfig().key_file;
    mqttConfig["enabled_websocket"] =
        _deviceConfig.getMQTTConfig().enabled_websocket;
    mqttConfig["websocket_path"] = _deviceConfig.getMQTTConfig().websocket_path;

    JsonArray pub_topics = mqttConfig.createNestedArray("pub_topic");
    for (auto& topic : _deviceConfig.getMQTTConfig().pub_topics) {
      pub_topics.add(topic);
    }

    JsonArray sub_topics = mqttConfig.createNestedArray("sub_topic");
    for (auto& topic : _deviceConfig.getMQTTConfig().sub_topics) {
      sub_topics.add(topic);
    }

    mqttConfig["mqtt_task_stack_size"] =
        _deviceConfig.getMQTTConfig().mqtt_task_stack_size;

    serializeJsonPretty(doc, Serial);
    Serial.println();
    _client.setConfig(mqttConfig);
    //* End MQTT Configuration
    log_i("[BasicMQTT]: Hostname: %s",
          _projectConfig.getMDNSConfig().hostname.c_str());
    _client.setup();
    //* Local Mosquitto Connection -- Start
  }
}

void BaseMQTT::onTopicUpdate(MQTTClient* client,
                             const mqtt_client_topic_data* topic) {}

void BaseMQTT::onConnected(MQTTClient* client) {}

//* Handles messages arrived on subscribed topic(s)
void BaseMQTT::onDataReceived(MQTTClient* client,
                              const mqtt_client_event_data* data) {
  //* Convert the payload to a string
  std::string result;
  for (int i = 0; i < data->data_len; i++) {
    log_i("%s", static_cast<char>(data->data[i]));
    result += static_cast<char>(data->data[i]);
  }
  log_i("[BasicMQTT]: Message: [%s]", result.c_str());

  log_i(
      "[BasicMQTT]: [%lu] +++ MQTT message of size %d on topic "
      "%s: "
      "%.*s\r\n",
      millis(), data->data_len, data->topic.c_str(), data->data_len,
      data->data);

  // TODO: Add support to handle sensor specific commands
}

void BaseMQTT::onSubscribed(MQTTClient* thisClient,
                            const mqtt_client_topic_data* topic) {}

void BaseMQTT::dataHandler(const std::string& topic,
                           const std::string& payload) {
  log_d("[BasicMQTT]: Payload: %s", topic.c_str());
  if (!_client.connected() && !topic.empty()) {
    _client.addTopicSub(topic.c_str(), 2);
  }

  if (!topic.empty() && !payload.empty()) {
    _client.publish(topic.c_str(), payload.c_str(), payload.length(), 2, 1);
  }
}

void BaseMQTT::dataHandler(const std::string& topic, float payload) {
  log_d("[BasicMQTT]: Payload: %s", topic.c_str());
  if (!_client.connected() && !topic.empty()) {
    _client.addTopicSub(topic.c_str(), 2);
  }

  std::string payloadStr = std::to_string(payload);
  if (!topic.empty() && !payloadStr.empty()) {
    _client.publish(topic.c_str(), payloadStr.c_str(), payloadStr.length(), 2,
                    1);
  }
}

void BaseMQTT::dataHandler(const std::string& topic,
                           std::vector<float> payload) {
  log_d("[BasicMQTT]: Payload: %s", topic.c_str());
  if (!_client.connected() && !topic.empty()) {
    _client.addTopicSub(topic.c_str(), 2);
  }

  std::string payloadStr;
  for (auto& i : payload) {
    payloadStr += std::to_string(i) + ",";
    log_i("[BasicMQTT]: Payload: %s", payloadStr.c_str());
  }
  if (!topic.empty() && !payloadStr.empty()) {
    _client.publish(topic.c_str(), payloadStr.c_str(), payloadStr.length(), 2,
                    1);
  }
}

void BaseMQTT::dataHandler(const std::string& topic,
                           std::vector<std::string> payload) {
  log_d("[BasicMQTT]: Payload: %s", topic.c_str());
  if (!_client.connected() && !topic.empty()) {
    _client.addTopicSub(topic.c_str(), 2);
  }

  std::string payloadStr;
  for (auto& i : payload) {
    payloadStr += i + ",";
  }
  if (!topic.empty() && !payloadStr.empty()) {
    _client.publish(topic.c_str(), payloadStr.c_str(), payloadStr.length(), 2,
                    1);
  }
}

void BaseMQTT::dataHandler(const std::string& topic,
                           std::unordered_map<std::string, float> payload) {
  log_d("[BasicMQTT]: Payload: %s", topic.c_str());
  if (!_client.connected() && !topic.empty()) {
    _client.addTopicSub(topic.c_str(), 2);
  }

  std::string payloadStr;
  for (auto& i : payload) {
    payloadStr += i.first + ":" + std::to_string(i.second) + ",";
  }
  if (!topic.empty() && !payloadStr.empty()) {
    _client.publish(topic.c_str(), payloadStr.c_str(), payloadStr.length(), 2,
                    1);
  }
}

//******************************************************************************
// * Function: Discover mDNS Broker
// * Description: Find the mDNS broker and and port return them
// * Parameters: None
// * Return: int - 1 if success, 0 if fail
//******************************************************************************
bool BaseMQTT::discovermDNSBroker() {
  IPAddress mqttServer;
  //* check if there is a WiFi connection
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
