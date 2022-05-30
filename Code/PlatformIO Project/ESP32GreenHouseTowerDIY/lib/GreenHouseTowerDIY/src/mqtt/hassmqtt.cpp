#include "hassmqtt.hpp"
#include <iostream>
#include <functional>
using namespace std;

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
 * <node_id> (Optional): ID of the node providing the topic, this is not used by Home Assistant but may be used to structure the MQTT topic.
 * The ID of the node must only consist of characters from the character class [a-zA-Z0-9_-] (alphanumerics, underscore and hyphen).
 * <object_id>: The ID of the device. This is only to allow for separate topics for each device and is not used for the entity_id.
 * The ID of the device must only consist of characters from the character class [a-zA-Z0-9_-] (alphanumerics, underscore and hyphen).
 * Best practice for entities with a unique_id is to set <object_id> to unique_id and omit the <node_id>.
 **/
/* #define MQTT_DISCOVERY_PREFIX "homeassistant/"

bool mqttProcessing = false; */

unsigned long lastReadAt = millis();
unsigned long lastAvailabilityToggleAt = millis();
bool lastInputState = false;
unsigned long lastSentAt = millis();

#if !ENABLE_MDNS_SUPPORT
#define BROKER_ADDR IPAddress(192, 168, 0, 17) // IP address of the MQTT broker - change to your broker IP address or enable MDNS support
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT "No"))
#else
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT "Yes"))
#endif // !ENABLE_MDNS_SUPPORT

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);
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
HASSMQTT::HASSMQTT()
{
    pump_relay_pin = PUMP_RELAY_PIN;
#if ENABLE_PH_SUPPORT
    pHTopic = PH_TOPIC;
    pHOutTopic = PH_OUT_TOPIC;
    phDnPIN = PH_DN_PIN;
    phUpPIN = PH_UP_PIN;
    doseTimeSm = DOSE_TIME_SM;
    doseTimeMed = DOSE_TIME_MED;
    doseTimeLg = DOSE_TIME_LG;
#endif // ENABLE_PH_SUPPORT
}

HASSMQTT::~HASSMQTT()
{
}

void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length)
{
    // This callback is called when message from MQTT broker is received.
    // Please note that you should always verify if the message's topic is the one you expect.
    // For example: if (memcmp(topic, "myCustomTopic") == 0) { ... }
    log_i("New message on topic: %s", topic);
    log_i("Data: %s", (const char *)payload);
#if ENABLE_PH_SUPPORT
    if (memcmp(topic, hassmqtt.pHOutTopic, strlen(hassmqtt.pHOutTopic)) == 0)
    {
        // The message is for the pH sensor.
        // The message is in the format:
        // <pH>:<pH_value>
        String result;
        for (int i = 0; i < length; i++)
        {
            log_i("%s", (char)payload[i]);
            result += (char)payload[i];
        }

        // Act on the message
        if (result.equalsIgnoreCase("UP MED"))
        {
            log_i("Adjust pH UP a medium amount!");
            digitalWrite(hassmqtt.phUpPIN, HIGH);
            delay(hassmqtt.doseTimeMed);
            digitalWrite(hassmqtt.phUpPIN, LOW);
        }
        else if (result.equalsIgnoreCase("DOWN MED"))
        {
            log_i("Adjusting pH DOWN a medium amount!");
            digitalWrite(hassmqtt.phDnPIN, HIGH);
            delay(hassmqtt.doseTimeMed);
            digitalWrite(hassmqtt.phDnPIN, LOW);
        }
        else if (result.equalsIgnoreCase("UP SM"))
        {
            log_i("Adjust pH UP a small amount!");
            digitalWrite(hassmqtt.phUpPIN, HIGH);
            delay(hassmqtt.doseTimeSm);
            digitalWrite(hassmqtt.phUpPIN, LOW);
        }
        else if (result.equalsIgnoreCase("DOWN SM"))
        {
            log_i("Adjusting pH DOWN a small amount!");
            digitalWrite(hassmqtt.phDnPIN, HIGH);
            delay(hassmqtt.doseTimeSm);
            digitalWrite(hassmqtt.phDnPIN, LOW);
        }
        if (result.equalsIgnoreCase("UP LG"))
        {
            log_i("Adjust pH UP a large amount!");
            digitalWrite(hassmqtt.phUpPIN, HIGH);
            delay(hassmqtt.doseTimeLg);
            digitalWrite(hassmqtt.phUpPIN, LOW);
        }
        else if (result.equalsIgnoreCase("DOWN LG"))
        {
            log_i("Adjusting pH DOWN a large amount!");
            digitalWrite(hassmqtt.phDnPIN, HIGH);
            delay(hassmqtt.doseTimeLg);
            digitalWrite(hassmqtt.phDnPIN, LOW);
        }
    }
#endif // ENABLE_PH_SUPPORT

    mqtt.publish("greenhouse_tower_pub", "hello");
}

void onMqttConnected()
{
    log_i("Connected to the broker!");
    log_i("Subscribing to the topic: greenhouse_tower_pump_sub");
    // You can subscribe to custom topic if you need
    mqtt.subscribe("greenhouse_tower_pump_sub");
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
        cfg.config.relays[i] = (cfg.config.relays[i] == true) ? false : true;
    }
}

#if ENABLE_PH_SUPPORT
void onPHStateChanged(bool state, HASwitch *s)
{
    // PH Control
    bool ph = state;
}
#endif // ENABLE_PH_SUPPORT

// ############## functions to update current server settings ###################
#if ENABLE_MDNS_SUPPORT
int HASSMQTT::DiscovermDNSBroker()
{
    IPAddress mqttServer;
    // check if there is a WiFi connection
    if (WiFi.status() == WL_CONNECTED)
    {
        log_i("[mDNS Broker Discovery]: connected!\n");

        log_i("[mDNS Broker Discovery]: Setting up mDNS: ");
        if (!MDNS.begin(mqtt_mDNS_clientId))
        {
            log_i("[Fail]\n");
        }
        else
        {
            log_i("[OK]\n");
            log_i("[mDNS Broker Discovery]: Querying MQTT broker: ");

            int n = MDNS.queryService("mqtt", "tcp") || MDNS.queryService("_mqtt", "_tcp");

            if (n == 0)
            {
                // No service found
                log_i("[Fail]\n");
                return 0;
            }
            else
            {
                int mqttPort;
                // Found one or more MQTT services - use the first one.
                log_i("[OK]\n");
                mqttServer = MDNS.IP(0);
                mqttPort = MDNS.port(0);
                heapStr(&(cfg.config.MQTTBroker), mqttServer.toString().c_str());
                cfg.config.MQTTPort = mqttPort;

                switch (mqttPort)
                {
                case MQTT_PORT:
                    log_i("[mDNS Broker Discovery]: MQTT port is insecure - running on port: %d\n", mqttPort);
                    break;

                case MQTT_PORT_SECURE:
                    log_i("[mDNS Broker Discovery]: MQTT port is secure - running on port: %d\n", mqttPort);
                    break;

                case 0:
                    log_i("[mDNS Broker Discovery]: MQTT port is not set - running on port: %d\n", mqttPort);
                    break;

                default:
                    log_i("[mDNS Broker Discovery]: MQTT port is on an unusual port - running on port: %d\n", mqttPort);
                    break;
                }

                log_i("[mDNS Broker Discovery]: MQTT broker found at: %s\n: %d", cfg.config.MQTTBroker, cfg.config.MQTTPort);
                return 1;
            }
        }
    }
    return 0;
}
#endif // ENABLE_MDNS_SUPPORT

/**
 * @brief Check if the current hostname is the same as the one in the config file
 * Call in the Setup BEFORE the WiFi.begin()
 * @param None
 * @return None
 */
void HASSMQTT::loadMQTTConfig()
{
    log_i("Checking if hostname is set and valid");
    size_t size = sizeof(cfg.config.hostname);
    if (!cfg.isValidHostname(cfg.config.hostname, size - 1))
    {
        heapStr(&cfg.config.hostname, DEFAULT_HOSTNAME);
        cfg.setConfigChanged();
    }

    String MQTT_CLIENT_ID = generateDeviceID();
    char *mqtt_user = MQTT_USER;
    char *mqtt_pass = MQTT_PASS;
    char *mqtt_client_id = StringtoChar(MQTT_CLIENT_ID);
    heapStr(&cfg.config.MQTTUser, mqtt_user);
    heapStr(&cfg.config.MQTTPass, mqtt_pass);
    heapStr(&cfg.config.MQTTClientID, mqtt_client_id);
    WiFi.setHostname(cfg.config.hostname); // define hostname
    cfg.setConfigChanged();
    free(mqtt_client_id);

    log_i("Loaded config: hostname %s, MQTT enable relay %s, MQTT host %s, MQTT port %d, MQTT user %s, MQTT pass %s, MQTT topic %s, MQTT set topic %s, MQTT device name %s",
          cfg.config.hostname,
          cfg.config.MQTTBroker,
          cfg.config.MQTTPort,
          cfg.config.MQTTUser,
          cfg.config.MQTTPass,
          cfg.config.MQTTDeviceName);

    log_i("Loaded config: hostname %s", cfg.config.hostname);
}

void HASSMQTT::mqttSetup()
{
    lastInputState = digitalRead(pump_relay_pin);
    // Unique ID must be set!
    String mac = WiFi.macAddress();
    byte buf[100];
    mac.getBytes(buf, sizeof(buf));
    device.setUniqueId(buf, sizeof(buf));

    lastReadAt = millis();
    lastAvailabilityToggleAt = millis();

    /* for (int i = 0; i < 10; i++)
    {
        log_i("%c", buf[i]);
    }
    log_i("\n"); */
    // connect to broker
    log_i("Connecting to Broker");

    // set device's details (optional)
    device.setName(cfg.config.MQTTDeviceName);
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
    mqtt.onMessage(onMqttMessage);
    mqtt.onConnected(onMqttConnected);
    mqtt.onConnectionFailed(onMqttConnectionFailed);

    mqtt.setDiscoveryPrefix("Greenhouse_Tower");

#if !MQTT_SECURE
#if ENABLE_MDNS_SUPPORT
    DiscovermDNSBroker();
    String mqtt_broker = cfg.config.MQTTBroker;
    IPAddress mqtt_ip;
    mqtt_ip.fromString(mqtt_broker);
    mqtt.begin(mqtt_ip);
#else
    mqtt.begin(BROKER_ADDR);
#endif // ENABLE_MDNS_SUPPORT
#else
#if ENABLE_MDNS_SUPPORT
    DiscovermDNSBroker();
    String mqtt_broker = cfg.config.MQTTBroker;
    IPAddress mqtt_ip;
    mqtt_ip.fromString(mqtt_broker);
    mqtt.begin(mqtt_ip, cfg.config.MQTTUser, cfg.config.MQTTPass);
#else
    mqtt.begin(BROKER_ADDR, cfg.config.MQTTUser, cfg.config.MQTTPass);
#endif // ENABLE_MDNS_SUPPORT
#endif // !MQTT_SECURE
}

void HASSMQTT::mqttLoop()
{
    mqtt.loop();
    if ((millis() - lastReadAt) > 30)
    { // read in 30ms interval
        // library produces MQTT message if a new state is different than the previous one
        relay.setState(digitalRead(pump_relay_pin));
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
        tower_humidity_temp.setValue(accumulatedata.config.humidity_temp);
        tower_humidity.setValue(accumulatedata.config.humidity);
        water_temp.setValue(accumulatedata.config.temp_sensors[0]);
        light.setValue(ldr.getLux());
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

HASSMQTT hassmqtt;