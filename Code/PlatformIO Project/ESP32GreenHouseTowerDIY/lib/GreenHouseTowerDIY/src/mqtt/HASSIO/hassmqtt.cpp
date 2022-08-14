#include "hassmqtt.hpp"

/**
 * @brief Initialize the MQTT client
 * @param clientId The client ID to use
 * @param host The host to connect to
 * @param port The port to connect to
 * @param secure Whether to use TLS or not
 * @param callback The callback to use
 * <discovery_prefix>/<component>/[<node_id>/]<object_id>/config
 * <component>: One of the supported MQTT components, eg. binary_sensor.
 * <node_id> (Optional): ID of the node providing the topic, this is not used by Home Assistant but may be used to structure the MQTT topic.
 * The ID of the node must only consist of characters from the character class [a-zA-Z0-9_-] (alphanumerics, underscore and hyphen).
 * <object_id>: The ID of the device. This is only to allow for separate topics for each device and is not used for the entity_id.
 * The ID of the device must only consist of characters from the character class [a-zA-Z0-9_-] (alphanumerics, underscore and hyphen).
 * Best practice for entities with a unique_id is to set <object_id> to unique_id and omit the <node_id>.
 **/
//#define MQTT_DISCOVERY_PREFIX "homeassistant/"
// bool mqttProcessing = false;

IPAddress broker_ip;

HADevice device;
HAMqtt mqtt(*HASSMQTT::network->espClient, device);
HASwitch relay("pump_relay", false); // is unique ID.
HASensor water_temp("water_temp");
#if USE_DHT_SENSOR
HASensor tower_humidity("tower_humidity");
HASensor tower_humidity_temp("tower_humidity_temp");
#endif // USE_DHT_SENSOR
HASensor light("light");

#if USE_SHT31_SENSOR
HASensor sht31_humidity("tower_humidity_sht31");
HASensor sht31_humidity_temp("tower_humidity_temp_sht31");
HASensor sht31_humidity_2("tower_humidity_sht31");
HASensor sht31_humidity_temp_2("tower_humidity_temp_sht31");
#endif // USE_SHT31_SENSOR

/***********************************************************************************************************************
 * Class Global Variables
 * Please only make changes to the following class variables within the ini file. Do not change them here.
 **********************************************************************************************************************/

void basicCallback(const char *topic, const uint8_t *payload, uint16_t length)
{
    baseMQTT.callback(topic, payload, length);
}

void onMqttConnected();
void onBeforeStateChanged(bool state, HASwitch *s);
void onRelayStateChanged(bool state, HASwitch *s);
void onMqttConnectionFailed();
void onPHStateChanged(bool state, HASwitch *s);

HASSMQTT::HASSMQTT(Network *network,
                   Config *deviceConfig,
                   PUMP *pump,
                   Relays *relays,
                   AccumulateData *accumulateData,
                   PHSENSOR *phsensor,
                   NetworkNTP *ntp,
                   XMqttBaseClass *baseMQTT,
                   Humidity *humidity,
                   TowerTemp *tower_temp,
                   LDR *ldr) : network(network),
                               deviceConfig(deviceConfig),
                               pump(pump),
                               relays(relays),
                               accumulateData(accumulateData),
                               phsensor(phsensor),
                               ntp(ntp),
                               baseMQTT(baseMQTT),
                               humidity(humidity),
                               tower_temp(tower_temp),
                               ldr(ldr),
                               lastReadAt(millis()),
                               lastAvailabilityToggleAt(millis()),
                               lastInputState(digitalRead(pump->pump_data._pump_relay_pin)),
                               lastSentAt(millis()),
                               lastReconnectAttempt(0) {}

HASSMQTT::~HASSMQTT() {}

bool HASSMQTT::begin()
{
    baseMQTT->begin();
    // Unique ID must be set!
    String mac = WiFi.macAddress();
    byte buf[100];
    mac.getBytes(buf, sizeof(buf));
    device.setUniqueId(buf, sizeof(buf));

    for (int i = 0; i < 10; i++)
    {
        log_d("%c", buf[i]);
    }
    log_d("\n");

    // connect to broker
    log_i("Connecting to Broker");

    // set device's details (optional)
    strcmp(cfg.config.MQTTDeviceName, DEFAULT_HOSTNAME) != 0 ? device.setName(DEFAULT_HOSTNAME) : device.setName(cfg.config.MQTTDeviceName);
    device.setSoftwareVersion(String(VERSION).c_str());

    // handle switch state
    relay.onBeforeStateChanged(onBeforeStateChanged); // optional
    relay.onStateChanged(onRelayStateChanged);
    relay.setName("Pump Relay"); // optional

    // This method enables availability for all device types registered on the device.
    // For example, if you have 5 sensors on the same device, you can enable
    // shared availability and change availability state of all sensors using
    // single method call "device.setAvailability(false|true)"
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
#endif // USE_DHT_SENSOR

    water_temp.setUnitOfMeasurement("°C");
    water_temp.setDeviceClass("temperature");
    water_temp.setIcon("mdi:coolant-temperature");
    water_temp.setName("Tower water temperature");

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
#endif // USE_SHT31_SENSOR

    light.setUnitOfMeasurement("lx");
    light.setDeviceClass("illuminance");
    light.setIcon("mdi:lightbulb");
    light.setName("Light");
    mqtt.onMessage(basicCallback);
    mqtt.onConnected(onMqttConnected);
    mqtt.onConnectionFailed(onMqttConnectionFailed);

    mqtt.setDiscoveryPrefix("Greenhouse_Tower");

#if MQTT_SECURE
    bool success = mqtt.begin(broker_ip.fromString(baseMQTT->getBrokerAddress()), cfg.config.MQTTUser, cfg.config.MQTTPass);
#else
    bool success = mqtt.begin(broker_ip.fromString(baseMQTT->getBrokerAddress()));
#endif // MQTT_SECURE
    return success;
}

void onMqttConnected()
{
    // You can subscribe to custom topic if you need
    log_i("Connected to the broker!");
    log_i("Subscribing to the topic: %s", pump.pump_data._pumpTopic);
    mqtt.subscribe(pump.pump_data._pumpTopic);
    if (phsensor->ph_data.find("id") != phsensor->ph_data.end())
    {
        const PHSENSOR::ph_Data_t &phData = phsensor->ph_data.at("id");
        log_i("Subscribing to the topic: %s", phData._pHTopic);
        mqtt.subscribe(phData._pHTopic);
    }
}

void onMqttConnectionFailed()
{
    log_e("Failed to connect to the broker!");
}

void onBeforeStateChanged(bool state, HASwitch *s)
{
    // this callback will be called before publishing new state to HA
    // in some cases there may be delay before onStateChanged is call relay due to network latency
}

void onRelayStateChanged(bool state, HASwitch *s)
{
    // Relay Control
    bool relay = state;
    for (int i = 0; i < sizeof(cfg.config.relays_pin) / sizeof(cfg.config.relays_pin[0]); i++)
    {
        log_i("switching state of pin: %s\n", relay ? "HIGH" : "LOW");
        cfg.config.relays[i] = (cfg.config.relays[i]) ? false : true;
    }
}

#if ENABLE_PH_SUPPORT
void onPHStateChanged(bool state, HASwitch *s)
{
    // PH Control
    bool ph = state;
}
#endif // ENABLE_PH_SUPPORT

void HASSMQTT::mqttLoop()
{
    mqtt.loop();
    if ((millis() - lastReadAt) > 30)
    { // read in 30ms interval
        // library produces MQTT message if a new state is different than the previous one
        relay.setState(digitalRead(pump->pump_data._pump_relay_pin));
        lastInputState = relay.getState();
        lastReadAt = millis();
    }

    if ((millis() - lastAvailabilityToggleAt) > 5000)
    {
        device.setAvailability(!device.isOnline());
        lastAvailabilityToggleAt = millis();
    }

    if ((millis() - lastSentAt) >= 5000)
    {
        lastSentAt = millis();
        tower_humidity_temp.setValue(humidity->result.temp);
        tower_humidity.setValue(humidity->result.humidity);
        water_temp.setValue(tower_temp->temp_sensor_results.temp[0]);
        light.setValue(ldr->getLux());
#if USE_SHT31_SENSOR
        sht31_humidity.setValue(lastValue);
        sht31_humidity_temp.setValue(lastValue);
        sht31_humidity_2.setValue(lastValue);
        sht31_humidity_temp_2.setValue(lastValue);
#endif // USE_SHT31_SENSOR

        // Supported data types:
        // uint32_t (uint16_t, uint8_t)
        // int32_t (int16_t, int8_t)
        // double
        // float
        // const char*
    }
}