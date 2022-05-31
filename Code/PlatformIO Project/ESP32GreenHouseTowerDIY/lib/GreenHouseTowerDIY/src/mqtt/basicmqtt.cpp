#include "basicmqtt.hpp"

// Class Objects
WiFiClient client;

#if ENABLE_MDNS_SUPPORT
#define BROKER_ADDR cfg.config.MQTTBroker // IP address of the MQTT broker - change to your broker IP address or enable MDNS support
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT " - Yes"))
#else
#define BROKER_ADDR IPAddress(192, 168, 0, 17) // IP address of the MQTT broker - change to your broker IP address or enable MDNS support
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT " - No"))
#endif // !ENABLE_MDNS_SUPPORT

#if MQTT_SECURE
PubSubClient mqttClient(BROKER_ADDR, MQTT_SECURE_PORT, callback, client); // Local Mosquitto Connection
#else
PubSubClient mqttClient(BROKER_ADDR, MQTT_PORT, callback, client); // Local Mosquitto Connection
#endif // MQTT_SECURE

const long interval = 60000;
unsigned long previousMillis = 0;

uint8_t user_bytes_received = 0;
const uint8_t bufferlen = 32;
char user_data[bufferlen];

const long interval = 60000;
unsigned long previousMillis = 0;

uint8_t user_bytes_received = 0;
const uint8_t bufferlen = 32;
char user_data[bufferlen];

/***********************************************************************************************************************
 * Class Global Variables
 * Please only make changes to the following class variables within the ini file. Do not change them here.
 **********************************************************************************************************************/
BASEMQTT::BASEMQTT()
{
    pumpTopic = PUMP_TOPIC;

    pHTopic = PH_TOPIC;
    pHOutTopic = PH_OUT_TOPIC;
    phDnPIN = PH_DN_PIN;
    phUpPIN = PH_UP_PIN;
}

BASEMQTT::~BASEMQTT()
{
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
    if (strcmp(topic, basemqtt.pumpTopic) == 0)
    {
        if (strcmp(result.c_str(), "ON") == 0)
        {
            log_i("Turning on the pump");
            Relay.RelayOnOff(basemqtt.pump_relay_pin, true);
        }
        else if (strcmp(result.c_str(), "OFF") == 0)
        {
            log_i("Turning off the pump");
            Relay.RelayOnOff(basemqtt.pump_relay_pin, false);
        }
    }
    else if (strcmp(topic, basemqtt.pHTopic) == 0)
    {
        log_i("Setting pH level to: [%s]", result.c_str());
        phsensor.eventListener(topic, payload, length);
    }
}

void BASEMQTT::mqttSetup()
{
    log_i("Settings up MQTT...");

    // Local Mosquitto Connection -- Start
    if (mqttClient.connect(DEFAULT_HOSTNAME))
    {
        // connection succeeded
        log_i("Connection succeeded. Subscribing to the topic [%s]", pHTopic);
        mqttClient.subscribe(pHTopic);
        log_i("Successfully subscribed to the topic.");
    }
    else
    {
        // connection failed
        log_i("Connection failed. MQTT client state is: %d", mqttClient.state());
    } // Local Mosquitto Connection -- End
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
            mqttClient.subscribe(pHTopic);
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
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            if (Serial.available() > 0)
            {
                user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));
            }

            if (user_bytes_received)
            {
                phsensor.parse_cmd(user_data);
                user_bytes_received = 0;
                memset(user_data, 0, sizeof(user_data));
            }

            log_i("Sending message to topic: %s", pHOutTopic);

            float newpH = cfg.config.pH;
            String timeStamp = networkntp.getTimeStamp();
            log_i("pH: %s", String(newpH).c_str());
            mqttClient.publish(pHOutTopic, String(newpH).c_str(), true);
            mqttClient.publish(pHOutTopic, timeStamp.c_str(), true);
        }
    }
}

BASEMQTT basemqtt;