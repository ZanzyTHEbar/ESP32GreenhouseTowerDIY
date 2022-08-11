#include "basicMqtt.hpp"

//***********************************************************************************************************************
// * Basic MQTT Class
// * This class is used to connect to a MQTT broker and publish and subscribe to topics.
//************************************************************************************************************************
IPAddress broker_ip;
void basicCallback(char *topic, byte *payload, unsigned int length);

#if MQTT_SECURE
PubSubClient mqttClient(broker_ip.fromString(baseMQTT.getBrokerAddress()), MQTT_SECURE_PORT, callback, *network.espClient); // Local Mosquitto Connection
#else
PubSubClient mqttClient(broker_ip.fromString(baseMQTT.getBrokerAddress()), MQTT_PORT, basicCallback, *network.espClient); // Local Mosquitto Connection
#endif // MQTT_SECURE

void basicCallback(char *topic, byte *payload, unsigned int length)
{
    baseMQTT.callback(topic, payload, length);
}

BasicMqtt::BasicMqtt() : _interval(60000),
                         _interval_reconnect(5000),
                         _user_data{0},
                         _previousMillis(0),
                         _user_bytes_received(0) {}

BasicMqtt::~BasicMqtt() {}

bool BasicMqtt::begin()
{
    baseMQTT.begin();
    // Local Mosquitto Connection -- Start
    if (!mqttClient.connect(DEFAULT_HOSTNAME))
    {
        // connection failed
        log_i("Connection failed. MQTT client state is: %d", mqttClient.state());
        StateManager_MQTT.setState(ProgramStates::DeviceStates::MQTTState_e::MQTT_Error);
        cfg.config.MQTTConnectedState = mqttClient.state();
        return false;
    }

    cfg.config.MQTTConnectedState = mqttClient.state();
    StateManager_MQTT.setState(ProgramStates::DeviceStates::MQTTState_e::MQTT_Connected);
#if ENABLE_PH_SUPPORT
    // connection succeeded
    if (phsensor.ph_data.find("id") != phsensor.ph_data.end())
    {

        log_i("Connection succeeded. Subscribing to the topic [%s]", phData._pHTopic);
        mqttClient.subscribe(phData._pHTopic);
    }
#endif // ENABLE_PH_SUPPORT
    log_i("Subscribing to the topic [%s]", pump.pump_data._pumpTopic);
    mqttClient.subscribe(pump.pump_data._pumpTopic);

    log_i("Successfully subscribed to the topic.");
    /* _speakerTopic = SPEAKER_TOPIC;
    _waterlevelTopic = WATER_LEVEL_TOPIC; */
    return true;
}

void BasicMqtt::mqttReconnect()
{
    // Loop until we're reconnected
    if (StateManager_MQTT.getCurrentState() == ProgramStates::DeviceStates::MQTTState_e::MQTT_Disconnected)
    {
        log_i("Attempting MQTT connection...");
        // Attempt to connect
        if (mqttClient.connect(DEFAULT_HOSTNAME))
        {
            log_i("Connected to MQTT broker.");
            // Subscribe
#if ENABLE_PH_SUPPORT
            mqttClient.subscribe(phsensor._phData._pHTopic);
#endif // ENABLE_PH_SUPPORT
        }
        else
        {
            log_i("failed, rc= %d", mqttClient.state());
            log_i(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            my_delay(5L);
        }
    }
}

void BasicMqtt::mqttLoop()
{
    my_delay(1L);

    if (StateManager_MQTT.getCurrentState() == ProgramStates::DeviceStates::MQTTState_e::MQTT_Disconnected || StateManager_MQTT.getCurrentState() == ProgramStates::DeviceStates::MQTTState_e::MQTT_Error)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - _previousMillis >= _interval_reconnect)
        {
            _previousMillis = currentMillis;
            mqttReconnect();
        }
    }
    else
    {
        mqttClient.loop();
        // callback;

        unsigned long currentMillis = millis();
        if (currentMillis - _previousMillis >= _interval)
        {

            _previousMillis = currentMillis;

            log_i("Sending message to topic: %s", phsensor._phData._pHOutTopic);
            String timeStamp = networkntp.getTimeStamp();
            mqttClient.publish(phsensor._phData._pHOutTopic, timeStamp.c_str(), true);
        }
    }
}

void BasicMqtt::checkState()
{
    cfg.config.MQTTConnectedState = mqttClient.connected();

    cfg.config.MQTTConnectedState ? StateManager_MQTT.setState(ProgramStates::DeviceStates::MQTTState_e::MQTT_Connected) : StateManager_MQTT.setState(ProgramStates::DeviceStates::MQTTState_e::MQTT_Disconnected);
    log_i("MQTT client state is: %d", mqttClient.state());
}

BasicMqtt basicmqtt;
