#include "hassmqtt.hpp"

long lastReconnectAttempt = 0;

/**
 * @brief Initialize the MQTT client
 * @param clientId The client ID to use
 * @param host The host to connect to
 * @param port The port to connect to
 * @param secure Whether to use TLS or not
 * @param callback The callback to use
 * <discovery_prefix>/<component>/[<node_id>/]<object_id>/config
 * <component>: One of the supported MQTT components, eg. binary_sensor.
 * <node_id> (Optional): ID of the node providing the topic, this is not used by
 *Home Assistant but may be used to structure the MQTT topic. The ID of the node
 *must only consist of characters from the character class [a-zA-Z0-9_-]
 *(alphanumerics, underscore and hyphen). <object_id>: The ID of the device.
 *This is only to allow for separate topics for each device and is not used for
 *the entity_id. The ID of the device must only consist of characters from the
 *character class [a-zA-Z0-9_-] (alphanumerics, underscore and hyphen). Best
 *practice for entities with a unique_id is to set <object_id> to unique_id and
 *omit the <node_id>.
 **/
// #define MQTT_DISCOVERY_PREFIX "homeassistant/"

HASSMQTT::HASSMQTT(WiFiClient* espClient,
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
      broker_ip(broker_ip),
      deviceConfig(config),
      device(),
      mqtt(*espClient, device),
#if USE_DHT_SENSOR
      tower_humidity("tower_humidity"),
      tower_humidity_temp("tower_humidity_temp"),
#endif  // USE_DHT_SENSOR
      light("light"),
#if USE_SHT31_SENSOR
      sht31_humidity("tower_humidity_sht31"),
      sht31_humidity_temp("tower_humidity_temp_sht31"),
      sht31_humidity_2("tower_humidity_sht31"),
      sht31_humidity_temp_2("tower_humidity_temp_sht31"),
#endif  // USE_SHT31_SENSOR
      tower_temp("water_temp"),
      water_level("water_level"),
      ph("ph"),
      lastReadAt(0),
      lastSentAt(0),
      lastInputState(0),
      lastAvailabilityToggleAt(0) {
}

HASSMQTT::~HASSMQTT() {}

void HASSMQTT::begin() {
  //! Unique ID must be set!

  //* Set device's unique ID
  String mac = WiFi.macAddress();
  byte buf[100];
  mac.getBytes(buf, sizeof(buf));
  device.setUniqueId(buf, sizeof(buf));
  log_d("%s", mac.c_str());

  //* connect to broker
  log_i("Connecting to Broker");

  //* set device's details (optional)
  auto mdnsConfig = deviceConfig->getMDNSConfig();

  device.setName(mdnsConfig->hostname == DEFAULT_HOSTNAME
                     ? DEFAULT_HOSTNAME
                     : mdnsConfig->hostname.c_str());
  device.setManufacturer("ESP32GreenHouseTowerDIY");

  // TODO: add support for version by injecting version
  // device.setSoftwareVersion(VERSION);

  //* handle relay states
  // create relays using the relay board instance
  // and set their unique IDs
  auto relayConfig = deviceConfig->getRelaysConfig();
  for (auto& relayEntity : *relayConfig) {
    relayEntity.ha_switch->setName(relayEntity.name.c_str());  // optional

    relayEntity.ha_switch->onBeforeStateChanged(
        [&](bool state, HASwitch* s) { onBeforeStateChanged(state, s); });
    relayEntity.ha_switch->onStateChanged(
        [&](bool state, HASwitch* s) { onRelayStateChanged(state, s); });
  }

  // This method enables availability for all device types registered on the
  // device. For example, if you have 5 sensors on the same device, you can
  // enable shared availability and change availability state of all sensors
  // using single method call "device.setAvailability(false|true)"
  device.enableSharedAvailability();

  // Optionally, you can enable MQTT LWT feature. If device will lose connection
  // to the broker, all device types related to it will be marked as offline in
  // the Home Assistant Panel.
  device.enableLastWill();

  // configure sensors
#if USE_DHT_SENSOR
  tower_humidity_temp.setUnitOfMeasurement("°C");
  tower_humidity_temp.setDeviceClass("temperature");
  tower_humidity_temp.setIcon("mdi:thermometer-lines");
  tower_humidity_temp.setName("Tower temperature");

  tower_humidity.setUnitOfMeasurement("%");
  tower_humidity.setDeviceClass("humidity");
  tower_humidity.setIcon("mdi:water-percent");
  tower_humidity.setName("Tower Humidity");
#endif  // USE_DHT_SENSOR

  tower_temp.setUnitOfMeasurement("°C");
  tower_temp.setDeviceClass("temperature");
  tower_temp.setIcon("mdi:coolant-temperature");
  tower_temp.setName("Tower water temperature");

#if USE_SHT31_SENSOR
  sht31_humidity.setUnitOfMeasurement("%");
  sht31_humidity.setDeviceClass("humidity");
  sht31_humidity.setIcon("mdi:water-percent");
  sht31_humidity.setName("Tower Humidity");

  sht31_humidity_temp.setUnitOfMeasurement("°C");
  sht31_humidity_temp.setDeviceClass("temperature");
  sht31_humidity_temp.setIcon("mdi:thermometer-lines");
  sht31_humidity_temp.setName("Tower Temperature");

  sht31_humidity_2.setUnitOfMeasurement("%");
  sht31_humidity_2.setDeviceClass("humidity");
  sht31_humidity_2.setIcon("mdi:water-percent");
  sht31_humidity_2.setName("Tower Humidity Sensor 2");

  sht31_humidity_temp_2.setUnitOfMeasurement("°C");
  sht31_humidity_temp_2.setDeviceClass("humidity");
  sht31_humidity_temp_2.setIcon("mdi:thermometer-lines");
  sht31_humidity_temp_2.setName("Tower Temperature Sensor 2");
#endif  // USE_SHT31_SENSOR

  light.setUnitOfMeasurement("lx");
  light.setDeviceClass("illuminance");
  light.setIcon("mdi:lightbulb");
  light.setName("Light");
  mqtt.onMessage(
      [&](const char* topic, const uint8_t* payload, uint16_t length) {
        onMqttMessage(topic, payload, length);
      });
  mqtt.onConnected([&]() { onMqttConnected(); });
  mqtt.onConnectionFailed([&]() { onMqttConnectionFailed(); });

  mqtt.setDiscoveryPrefix("Greenhouse_Tower");

#if MQTT_SECURE
  mqtt.begin(*broker_ip), cfg.config.MQTTUser,
             cfg.config.MQTTPass);
#else
  mqtt.begin(*broker_ip);
#endif  // MQTT_SECURE
}

void HASSMQTT::onMqttMessage(const char* topic,
                             const uint8_t* payload,
                             uint16_t length) {
  // This callback is called when message from MQTT broker is received.
  // Please note that you should always verify if the message's topic is the one
  // you expect. For example: if (memcmp(topic, "myCustomTopic") == 0) { ... }
  log_i("Message arrived on topic: [%s] ", topic);

  //* Convert the payload to a string
  std::string result;
  for (int i = 0; i < length; i++) {
    log_i("%s", static_cast<char>(payload[i]));
    result += static_cast<char>(payload[i]);
  }
  log_i("Message: [%s]", result.c_str());

  // TODO: Add support for all sensor topics

  //* BH1750

  //* LDR

  //* TowerTemp

  //* Humidity

  //* WaterLevelSensor

  //* Relays
  auto relayConfig = deviceConfig->getRelaysConfig();
  for (auto relay : *relayConfig) {
    if (relay.name == topic) {
      log_i("Setting relay [%s] to: [%s]", relay.name.c_str(), result.c_str());
      relayboard->eventListener(result, relay.port);
      return;
    }
  }

  //* Ph Sensor
#if ENABLE_PH_SUPPORT
  if (phsensor->_pHTopic == topic) {
    log_i("Setting pH level to: [%s]", result.c_str());
    phsensor->eventListener(result);
  }
#endif  // ENABLE_PH_SUPPORT

  mqtt.publish("greenhouse_tower_info", "Hello from the Greenhouse Tower!");
}

void HASSMQTT::onMqttConnected() {
  // Subscribe to all relay topics
  // TODO: Add ECC support based on return value of subscribe()

  // TODO: Add support for subscribing to all sensor topics

  //* BH1750

  //* LDR

  //* TowerTemp

  //* Humidity

  //* WaterLevelSensor

  //* Relays
  auto relayConfig = deviceConfig->getRelaysConfig();
  for (auto relay : *relayConfig) {
    log_i("Subscribing to the topic [%s]", relay.name.c_str());
    mqtt.subscribe(relay.name.c_str());
  }

  //* Ph Sensor
#if ENABLE_PH_SUPPORT
  // connection succeeded
  log_i("Connection succeeded. Subscribing to the topic [%s]",
        phsensor->_pHTopic.c_str());
  mqtt.subscribe(phsensor->_pHTopic.c_str());
#endif  // ENABLE_PH_SUPPORT
  log_i("Successfully subscribed to all topics.");
}

void HASSMQTT::onMqttConnectionFailed() {
  log_e("Failed to connect to the broker!");
}

void HASSMQTT::onBeforeStateChanged(bool state, HASwitch* s) {
  // this callback will be called before publishing new state to HA
  // in some cases there may be delay before onStateChanged is call relay due to
  // network latency
}

/**
 * @brief This callback will be called evertime the relay state changes
 *
 * @param state new state of the relay
 * @param s pointer to the relay object
 */
void HASSMQTT::onRelayStateChanged(bool state, HASwitch* s) {
  // Relay Control
  std::string relay = s->getName();
  log_i("Relay %s state changed to: [%s]", state ? "ON" : "OFF", relay.c_str());
}

#if ENABLE_PH_SUPPORT
void onPHStateChanged(bool state, HASwitch* s) {
  // PH Control
  bool ph = state;
}
#endif  // ENABLE_PH_SUPPORT

void HASSMQTT::mqttLoop() {
  mqtt.loop();
  if ((millis() - lastReadAt) > 30) {  // read in 30ms interval
    // library produces MQTT message if a new state is different than the
    // previous one

    auto relayConfig = deviceConfig->getRelaysConfig();
    for (auto relayEntity : *relayConfig) {
      relayEntity.ha_switch->setState(digitalRead(relayEntity.port));
      lastInputState = relayEntity.ha_switch->getState();
    }
    lastReadAt = millis();
  }

  if ((millis() - lastAvailabilityToggleAt) > 5000) {
    device.setAvailability(!device.isOnline());
    lastAvailabilityToggleAt = millis();
  }

  if ((millis() - lastSentAt) >= 5000) {
    lastSentAt = millis();
#if USE_DHT_SENSOR
    tower_humidity_temp.setValue(humidity->result.temp);
    tower_humidity.setValue(humidity->result.humidity);
#endif  // USE_DHT_SENSOR

    tower_temp.setValue(towertemp->temp_sensor_results.temp.at(0));
    light.setValue(ldr->getLux());

#if USE_SHT31_SENSOR
    sht31_humidity.setValue(humidity->result.humidity_sht31);
    sht31_humidity_2.setValue(humidity->result.humidity_sht31_2);
    sht31_humidity_temp.setValue(humidity->result.temp_sht31);
    sht31_humidity_temp_2.setValue(humidity->result.temp_sht31_2);
#endif  // USE_SHT31_SENSOR

    // Supported data types:
    // uint32_t (uint16_t, uint8_t)
    // int32_t (int16_t, int8_t)
    // double
    // float
    // const char*
  }
}
