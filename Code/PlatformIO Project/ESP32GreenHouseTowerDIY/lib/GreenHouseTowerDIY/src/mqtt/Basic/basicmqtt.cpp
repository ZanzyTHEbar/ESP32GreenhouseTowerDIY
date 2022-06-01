#include "basicmqtt.hpp"

#if ENABLE_MDNS_SUPPORT
#define BROKER_ADDR cfg.config.MQTTBroker // IP address of the MQTT broker - change to your broker IP address or enable MDNS support
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT " - Yes"))
#else
#define BROKER_ADDR MQTT_BROKER // IP address of the MQTT broker - change to your broker IP address or enable MDNS support
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT " - No"))
#endif // ENABLE_MDNS_SUPPORT

IPAddress broker_ip;

void callback(char *topic, byte *payload, unsigned int length);

#if MQTT_SECURE
PubSubClient mqttClient(broker_ip.fromString(BROKER_ADDR), MQTT_SECURE_PORT, callback, espClient); // Local Mosquitto Connection
#else
PubSubClient mqttClient(broker_ip.fromString(BROKER_ADDR), MQTT_PORT, callback, espClient); // Local Mosquitto Connection
#endif // MQTT_SECURE

//***********************************************************************************************************************
// * Class Global Variables
// * Please only make changes to the following class variables within the ini file. Do not change them here.
//************************************************************************************************************************

BASEMQTT::BASEMQTT() : _interval(60000), _user_data{0}, _previousMillis(0), _user_bytes_received(0)
{
    log_i("Setting up MQTT...");

    // Local Mosquitto Connection -- Start
    if (mqttClient.connect(DEFAULT_HOSTNAME))
    {
#if ENABLE_PH_SUPPORT
        // connection succeeded
        log_i("Connection succeeded. Subscribing to the topic [%s]", phsensor._pHTopic);
        mqttClient.subscribe(phsensor._pHTopic);
#endif // ENABLE_PH_SUPPORT
        log_i("Successfully subscribed to the topic.");
    }
    else
    {
        // connection failed
        log_i("Connection failed. MQTT client state is: %d", mqttClient.state());
    }
    // Local Mosquitto Connection -- End
}

BASEMQTT::~BASEMQTT()
{
    // Destructor
}

// Handles messages arrived on subscribed topic(s)
void callback(char *topic, byte *payload, unsigned int length)
{
    String result;
    log_i("Message arrived on topic: [%s] ", topic);
    for (int i = 0; i < length; i++)
    {
        log_i("payload is: [%s]", (char)payload[i]);
        result += (char)payload[i];
    }
    log_i("Message: [%s]", result.c_str());

    // Check if the message is for the current topic
    if (strcmp(topic, pump._pumpTopic) == 0)
    {
        if (strcmp(result.c_str(), "ON") == 0)
        {
            log_i("Turning on the pump");
            Relay.RelayOnOff(pump._pump_relay_pin, true);
        }
        else if (strcmp(result.c_str(), "OFF") == 0)
        {
            log_i("Turning off the pump");
            Relay.RelayOnOff(pump._pump_relay_pin, false);
        }
    }
#if ENABLE_PH_SUPPORT
    else if (strcmp(topic, phsensor._pHTopic) == 0)
    {
        log_i("Setting pH level to: [%s]", result.c_str());
        phsensor.eventListener(topic, payload, length);
    }
#endif // ENABLE_PH_SUPPORT
}

void BASEMQTT::loadMQTTConfig()
{
    log_i("Checking if hostname is set and valid");
    size_t size = sizeof(cfg.config.hostname);
    if (!cfg.isValidHostname(cfg.config.hostname, size - 1))
    {
        heapStr(&cfg.config.hostname, DEFAULT_HOSTNAME);
        cfg.setConfigChanged();
    }

    String MQTT_CLIENT_ID = generateDeviceID();
    const char *mqtt_user = MQTT_USER;
    const char *mqtt_pass = MQTT_PASS;
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

void BASEMQTT::mqttReconnect()
{
    // Loop until we're reconnected
    while (!mqttClient.connected())
    {
        log_i("Attempting MQTT connection...");
        // Attempt to connect
        if (mqttClient.connect(DEFAULT_HOSTNAME))
        {
            log_i("Connected to MQTT broker.");
            // Subscribe
#if ENABLE_PH_SUPPORT
            mqttClient.subscribe(phsensor._pHTopic);
#endif // ENABLE_PH_SUPPORT
        }
        else
        {
            log_i("failed, rc= %d", mqttClient.state());
            log_i(" try again in 5 seconds");
            // Wait 15 seconds before retrying
            my_delay(15L);
        }
    }
}

void BASEMQTT::mqttLoop()
{
    my_delay(1L);

    if (!mqttClient.connected())
    {
        mqttReconnect();
    }
    else
    {
        mqttClient.loop();
        callback;

        unsigned long currentMillis = millis();
        if (currentMillis - _previousMillis >= _interval)
        {
            _previousMillis = currentMillis;

            if (Serial.available() > 0)
            {
                _user_bytes_received = Serial.readBytesUntil(13, _user_data, sizeof(_user_data));
            }

            if (_user_bytes_received)
            {
                phsensor.parse_cmd(_user_data);
                _user_bytes_received = 0;
                memset(_user_data, 0, sizeof(_user_data));
            }
#if ENABLE_PH_SUPPORT
            log_i("Sending message to topic: %s", phsensor._pHOutTopic);
#endif // ENABLE_PH_SUPPORT
            float newpH = cfg.config.pH;
            String timeStamp = networkntp.getTimeStamp();
            log_i("pH: %s", String(newpH).c_str());
#if ENABLE_PH_SUPPORT
            mqttClient.publish(phsensor._pHOutTopic, String(newpH).c_str(), true);
            mqttClient.publish(phsensor._pHOutTopic, timeStamp.c_str(), true);
#endif // ENABLE_PH_SUPPORT
        }
    }
}

BASEMQTT basemqtt;
