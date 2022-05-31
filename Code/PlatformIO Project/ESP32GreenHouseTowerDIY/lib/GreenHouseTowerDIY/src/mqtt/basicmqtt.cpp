#include "basicmqtt.hpp"

// Class Objects
WiFiClient client;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

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

int status = WL_IDLE_STATUS; // the Wifi radio's status

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
    pump_relay_pin = PUMP_RELAY_PIN;
    pHTopic = PH_TOPIC;
    pHOutTopic = PH_OUT_TOPIC;
    phDnPIN = PH_DN_PIN;
    phUpPIN = PH_UP_PIN;

    // Variables to save date and time
    formattedDate = "";
    dayStamp = "";
    timeStamp = "";
}

BASEMQTT::~BASEMQTT()
{
}

// Handles messages arrived on subscribed topic(s)
void callback(char *topic, byte *payload, unsigned int length)
{
    String result;
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        result += (char)payload[i];
    }
    Serial.println("");
}

void BASEMQTT::mqttSetup()
{
    pinMode(phUpPIN, OUTPUT);
    pinMode(phDnPIN, OUTPUT);

    Serial.print("You're connected to the network");
    // WiFi Connection -- End

    // Local Mosquitto Connection -- Start
    if (mqttClient.connect("arduino"))
    {
        // connection succeeded
        Serial.println("Connection succeeded.");
        Serial.print("Subscribing to the topic [");
        Serial.print(pHTopic);
        Serial.println("]");
        mqttClient.subscribe(pHTopic);
        Serial.println("Successfully subscribed to the topic.");
    }
    else
    {
        // connection failed
        Serial.print("Connection failed. MQTT client state is: ");
        Serial.println(mqttClient.state());
    } // Local Mosquitto Connection -- End

    // Initialize a NTPClient to get time
    timeClient.begin();
    // Set offset time in seconds to adjust for your timezone, for example:
    // GMT +1 = 3600
    // GMT +8 = 28800
    // GMT -1 = -3600
    // GMT 0 = 0
    timeClient.setTimeOffset(3600);
}

void BASEMQTT::mqttReconnect()
{
    // Loop until we're reconnected
    while (!mqttClient.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (mqttClient.connect("arduino"))
        {
            Serial.println("connected");
            // Subscribe
            mqttClient.subscribe(pHTopic);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 15 seconds before retrying
            delay(15000);
        }
    }
}

void BASEMQTT::mqttLoop()
{
    if (!timeClient.update())
    {
        timeClient.forceUpdate();
    }
    // The formattedDate comes with the following format:
    // 2022-05-28T16:00:13Z
    // We need to extract date and time
    formattedDate = timeClient.getFormattedDate();
    Serial.println(formattedDate);

    // Extract date
    int splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
    Serial.print("DATE: ");
    Serial.println(dayStamp);
    // Extract time
    timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
    Serial.print("HOUR: ");
    Serial.println(timeStamp);

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
                parse_cmd(user_data);
                user_bytes_received = 0;
                memset(user_data, 0, sizeof(user_data));
            }

            Serial.print("Sending message to topic: ");
            Serial.println(pHOutTopic);

            float newpH = cfg.config.pH;

            Serial.print("pH: ");
            Serial.println(String(newpH).c_str());
            mqttClient.publish(pHOutTopic, String(newpH).c_str(), true); // UTC.dateTime doesn't compile
            mqttClient.publish(pHOutTopic, timeStamp.c_str(), true);

            Serial.println();
        }
    }
}

BASEMQTT basemqtt;