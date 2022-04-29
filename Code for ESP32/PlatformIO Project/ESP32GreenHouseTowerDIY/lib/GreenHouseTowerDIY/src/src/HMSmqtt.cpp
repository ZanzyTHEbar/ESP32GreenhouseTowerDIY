
#include "HMSmqtt.hpp"

PubSubClient mqttClient(espClient);

long lastReconnectAttempt = 0;

HMSMqtt::HMSMqtt()
{
}

HMSMqtt::~HMSMqtt()
{
}

// ############## functions to update current server settings ###################
/**
 * @brief Check if the current hostname is the same as the one in the config file
 * Call in the Setup BEFORE the WiFi.begin()
 * @param None
 * @return None
 */
void HMSMqtt::loadMQTTConfig()
{
  log_i("Checking if hostname is set and valid");
  size_t size = sizeof(cfg.config.hostname);
  if (!cfg.isValidHostname(cfg.config.hostname, size - 1))
  {
    heapStr(&cfg.config.hostname, DEFAULT_HOSTNAME);
    cfg.setConfigChanged();
  }
  String MQTT_CLIENT_ID = generateDeviceID();
  cfg.config.MQTTEnabled = ENABLE_MQTT_SUPPORT;
  char *mqtt_user = StringtoChar(MQTT_USER);
  char *mqtt_pass = StringtoChar(MQTT_PASS);
  char *mqtt_topic = StringtoChar(MQTT_TOPIC);
  char *mqtt_topic_set = StringtoChar(MQTT_HOMEASSISTANT_TOPIC_SET);
  char *mqtt_device_name = StringtoChar(MQTT_DEVICE_NAME);
  char *mqtt_client_id = StringtoChar(MQTT_CLIENT_ID);
  heapStr(&cfg.config.MQTTUser, mqtt_user);
  heapStr(&cfg.config.MQTTPass, mqtt_pass);
  heapStr(&cfg.config.MQTTTopic, mqtt_topic);
  heapStr(&cfg.config.MQTTSetTopic, mqtt_topic_set);
  heapStr(&cfg.config.MQTTDeviceName, mqtt_device_name);
  heapStr(&cfg.config.MQTTClientID, mqtt_client_id);
  WiFi.setHostname(cfg.config.hostname); // define hostname
  cfg.setConfigChanged();
  free(mqtt_user);
  free(mqtt_pass);
  free(mqtt_topic);
  free(mqtt_topic_set);
  free(mqtt_device_name);
  free(mqtt_client_id);

  log_i("Loaded config: hostname %s, MQTT enabled %s, MQTT host %s, MQTT port %d, MQTT user %s, MQTT pass %s, MQTT topic %s, MQTT set topic %s, MQTT device name %s",
        cfg.config.hostname,
        (cfg.config.MQTTEnabled == ENABLE_MQTT_SUPPORT) ? "true" : "false",
        cfg.config.MQTTBroker,
        cfg.config.MQTTPort,
        cfg.config.MQTTUser,
        cfg.config.MQTTPass,
        cfg.config.MQTTTopic,
        cfg.config.MQTTSetTopic,
        cfg.config.MQTTDeviceName);

  log_i("Loaded config: hostname %s", cfg.config.hostname);
}

//############################## MQTT HELPER FUNCTIONS ##############################
void mqttSendStatus(String doc)
{
  StaticJsonDocument<1024> Doc;
  size_t b = sizeof(doc);
  deserializeJson(Doc, doc.c_str(), b);
  char buffer[256];
  size_t n = serializeJson(Doc, buffer);
  if (!mqttProcessing)
  {
    char *topic_data;
    const char *json = "/json_data";
    topic_data = appendChartoChar(cfg.config.MQTTTopic, json); // append string two to the result.
    mqttClient.publish(topic_data, buffer, n);
    log_i("Sending MQTT package: %s", buffer);
    mqttProcessing = false;
    free(topic_data);
  }
}

void callback(char *topic, byte *message, unsigned int length)
{
  mqttProcessing = true;
  log_i("Message arrived on topic: [%s]. Message: ", topic);
  for (int i = 0; i < length; i++)
  {
    log_i("%s\n", (char)message[i]);
  }
  log_i("\n");

  StaticJsonDocument<1024> doc;
  deserializeJson(doc, message, length);

  // log_iF("Received MQTT package: %s", doc.as<String>())

  JsonObject obj = doc.as<JsonObject>();
  for (JsonPair p : obj)
  {
    const char *key = p.key().c_str();
    JsonVariant v = p.value();

    if (strcmp(key, "state") == 0)
    {
      String val = v.as<String>();
      val.toLowerCase();
      if (val == String("on"))
      {
        // setPower(1);
      }
      else if (val == String("off"))
      {
        // setPower(0);
      }
      else if (val == String("toggle"))
      {
        // setPower((power == 1) ? 0 : 1);
      }
    }
  }
  mqttProcessing = false;
  String Doc = doc.as<String>();
  mqttSendStatus(Doc);
}
//############################## MQTT HELPER FUNCTIONS END ############################HMS

bool HMSMqtt::ReConnect() // TODO:Call this in the Loop pinned to task for core 0
{
  // Loop until we're reconnected
  if (!mqttClient.connected())
  {
    log_i("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(cfg.config.MQTTDeviceName))
    {
      log_i("connected");
      // Subscribe
      /* char *topic_sub;
      const char *led = "/LED";
      topic_sub = appendChartoChar(cfg.config.MQTTTopic, led); // append string two to the result.
      mqttClient.subscribe(topic_sub); */

      // Publish
      /* char *topic_pub;
      const char *test = "/test";
      topic_pub = appendChartoChar(cfg.config.MQTTTopic, test); // append string two to the result.
      mqttClient.publish(topic_pub, "hello world"); */
      return mqttClient.connected();
    }
    else
    {
      log_i("failed, rc=%d trying again in 5 seconds", mqttClient.state());
      // Wait 5 seconds before retrying
      return mqttClient.connected();
    }
  }
  return mqttClient.connected();
}

void HMSMqtt::MessageReceived(char topic[], char payload[])
{
  log_i("incoming: %s - %s", topic, payload);
  // Note: Do not use the mqtt in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `mqtt.loop()`.
}

bool HMSMqtt::MQTTSetup()
{
  // MQTT client
  if (WiFi.status() != WL_CONNECTED)
  {
    log_e("WiFi not connected");
    log_e("MQTT setup failed! Please connect your device to a WiFi network.");
    return false;
  }
  else
  {
    auto getPort = [](bool secure)
    {
      return secure ? MQTT_PORT_SECURE : MQTT_PORT;
    };
    auto port = getPort(cfg.config.MQTTSecureState);
    auto host = cfg.config.MQTTBroker;
    mqttClient.setServer(host, port);
    mqttClient.setCallback(callback);
    lastReconnectAttempt = 0;
    return true;
  }
}

void HMSMqtt::MQTTPublish(char topic[], char payload[])
{
  if (MQTTLoop())
  {
    unsigned int len = strlen(payload);
    bool publish = mqttClient.publish(topic, payload, len);
    if (publish != false)
    {
      log_i("MQTT publish success!");
    }
    else
    {
      log_i("MQTT publish failed!");
    }
  }
  else
  {
    if (Serial.available() > 0)
    {
      log_i("MQTT not connected, not publishing. Attempting to reconnect...");
    }
  }
}

bool HMSMqtt::MQTTLoop()
{
  if (!mqttClient.connected())
  {
    long now = millis();
    if (now - lastReconnectAttempt > 5000)
    {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (ReConnect())
      {
        lastReconnectAttempt = 0;
      }
    }
  }
  else
  {
    // Client connected
    mqttClient.loop();
  }
  return mqttClient.connected();
}

void HMSMqtt::RunMqttService()
{
  bool mqttConnected = false;

  if (!mqttClient.connected() && cfg.config.MQTTEnabled)
  {
    log_i("MQTT not connected, attempting to reconnect...");
    MQTTLoop();
  }
  if (mqttClient.connected() && cfg.config.MQTTEnabled)
  {
    log_i("\n");
    log_i("Connecting to MQTT...");
    cfg.config.MQTTConnectedState = mqttConnected = true;

    log_i("MQTT connected!");

    if (MQTTLoop())
    {
      log_i("connected\n");

      log_i("Subscribing to MQTT Topics");

      char *mqttSetTopicS = NULL;
      heapStr(&mqttSetTopicS, "~");
      char *setTopic = NULL;
      heapStr(&setTopic, appendChartoChar(mqttSetTopicS, cfg.config.MQTTTopic));
      free(mqttSetTopicS);
      free(setTopic);

      StaticJsonDocument<1024> JSONencoder;
      JSONencoder["~"] = cfg.config.MQTTTopic,
      JSONencoder["name"] = cfg.config.MQTTDeviceName,
      JSONencoder["dev"]["ids"] = cfg.config.MQTTClientID,
      JSONencoder["dev"]["mf"] = "Prometheon Technologies",
      JSONencoder["dev"]["mdl"] = VERSION,
      JSONencoder["dev"]["name"] = "HMS",
      JSONencoder["stat_t"] = "~",
      JSONencoder["cmd_t"] = setTopic,
      JSONencoder["mDNS"] = ENABLE_MDNS_SUPPORT,
      JSONencoder["I2C_Scanner"] = ENABLE_I2C_SCANNER,
      JSONencoder["MQTT"] = true,
      JSONencoder["MQTT_Secure"] = cfg.config.MQTTSecureState,
      JSONencoder["MQTT_Broker"] = cfg.config.MQTTBroker,
      JSONencoder["MQTT_Port"] = cfg.config.MQTTPort,
      JSONencoder["MQTT_User"] = cfg.config.MQTTUser,
      JSONencoder["Device"] = "HBAT",
      JSONencoder["schema"] = "json";

      size_t n = measureJson(JSONencoder);
      char *mqttConfigTopic = NULL;
      heapStr(&mqttConfigTopic, appendChartoChar(cfg.config.MQTTTopic, "/config"));
      bool publish = mqttClient.beginPublish(mqttConfigTopic, n, true);

      if (publish == true)
      {
        log_i("Configuration Publishing Begun");
        if (serializeJson(JSONencoder, mqttClient) == n)
        {
          log_i("Configuration Sent");
        }
        if (mqttClient.endPublish())
        {
          log_i("Configuration Publishing Finished");
          String status = JSONencoder.as<String>();
          mqttSendStatus(status);
          log_i("Sending Initial Status");
        }
      }
      else
      {
        log_e("Error sending Configuration");
      }
      free(mqttConfigTopic);
    }
    else
    {
      log_e("failed with state %d\n", mqttClient.state());
    }
  }
  else
  {
    cfg.config.MQTTConnectedState = mqttConnected = false;
  }
}

HMSMqtt HMSmqtt;