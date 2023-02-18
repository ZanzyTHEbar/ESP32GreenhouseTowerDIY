#include "basicmqtt.hpp"

//***********************************************************************************************************************
// * Class Global Variables
// * Please only make changes to the following class variables within the ini file. Do not change them here.
//************************************************************************************************************************

BASEMQTT::BASEMQTT(WiFiClient *espClient,
                   PHSENSOR *phsensor,
                   BH1750 *bh1750,
                   LDR *ldr,
                   TowerTemp *towertemp,
                   Humidity *humidity,
                   WaterLevelSensor *waterlevelsensor,
                   IPAddress *broker_ip) : phsensor(phsensor),
                                           bh1750(bh1750),
                                           ldr(ldr),
                                           towertemp(towertemp),
                                           humidity(humidity),
                                           waterlevelsensor(waterlevelsensor),
#if MQTT_SECURE
                                           PubSubClient(
                                               *broker_ip, MQTT_SECURE_PORT, [&](char *topic, byte *payload, unsigned int length)
                                               {
                                                    log_i("MQTT Callback");
                                                    mqttCallback(topic, payload, length); },
                                               *espClient),
#else
                                           PubSubClient(
                                               *broker_ip, MQTT_PORT, [&](char *topic, byte *payload, unsigned int length)
                                               {
                                                    log_i("MQTT Callback");
                                                    mqttCallback(topic, payload, length); },
                                               *espClient),
#endif // MQTT_SECURE
                                           _interval(60000),
                                           _user_data{0},
                                           _previousMillis(0),
                                           _user_bytes_received(0)
{
}

BASEMQTT::~BASEMQTT()
{
    // Destructor
}

void BASEMQTT::begin()
{
    log_i("Setting up MQTT...");

    // Local Mosquitto Connection -- Start
    if (connect(DEFAULT_HOSTNAME))
    {
#if ENABLE_PH_SUPPORT
        // connection succeeded
        log_i("Connection succeeded. Subscribing to the topic [%s]", phsensor->_pHTopic.c_str());
        subscribe(phsensor->_pHTopic.c_str());
#endif // ENABLE_PH_SUPPORT
        log_i("Successfully subscribed to the topic.");
        return;
    }
    log_i("Connection failed. MQTT client state is: %d", state());
    // Local Mosquitto Connection -- End
}

// Handles messages arrived on subscribed topic(s)
void BASEMQTT::mqttCallback(char *topic, byte *payload, unsigned int length)
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
    else if (phsensor->_pHTopic == topic)
    {
        log_i("Setting pH level to: [%s]", result.c_str());
        phsensor->eventListener(topic, payload, length);
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
    while (!connected())
    {
        log_i("Attempting MQTT connection...");
        // Attempt to connect
        if (connect(DEFAULT_HOSTNAME))
        {
            log_i("Connected to MQTT broker.");
            // Subscribe
#if ENABLE_PH_SUPPORT
            subscribe(phsensor->_pHTopic.c_str());
#endif // ENABLE_PH_SUPPORT
        }
        else
        {
            log_i("failed, rc= %d", state());
            log_i(" try again in 5 seconds");
            // Wait 15 seconds before retrying
            Network_Utilities::my_delay(15L);
        }
    }
}

void BASEMQTT::mqttLoop()
{
    Network_Utilities::my_delay(1L);
    if (!connected())
    {
        mqttReconnect();
    }
    else
    {
        loop();
        mqttCallback;

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
                phsensor->parse_cmd(_user_data);
                _user_bytes_received = 0;
                memset(_user_data, 0, sizeof(_user_data));
            }
#if ENABLE_PH_SUPPORT
            log_i("Sending message to topic: %s", phsensor->_pHOutTopic.c_str());
#endif // ENABLE_PH_SUPPORT
            float newpH = cfg.config.pH;
            String timeStamp = networkntp.getTimeStamp();
            log_i("pH: %s", String(newpH).c_str());
#if ENABLE_PH_SUPPORT
            publish(phsensor->_pHOutTopic.c_str(), String(newpH).c_str(), true);
            publish(phsensor->_pHOutTopic.c_str(), timeStamp.c_str(), true);
#endif // ENABLE_PH_SUPPORT
        }
    }
}
