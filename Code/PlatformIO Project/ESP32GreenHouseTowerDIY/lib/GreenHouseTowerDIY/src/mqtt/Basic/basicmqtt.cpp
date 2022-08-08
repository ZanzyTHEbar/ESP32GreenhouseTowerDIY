#include "basicmqtt.hpp"

IPAddress broker_ip;

void callback(char *topic, byte *payload, unsigned int length);
const char *getBrokerAddress();

#if MQTT_SECURE
PubSubClient mqttClient(broker_ip.fromString(getBrokerAddress()), MQTT_SECURE_PORT, callback, espClient); // Local Mosquitto Connection
#else
PubSubClient mqttClient(broker_ip.fromString(getBrokerAddress()), MQTT_PORT, callback, espClient); // Local Mosquitto Connection
#endif // MQTT_SECURE

//***********************************************************************************************************************
// * Basic MQTT Class
// * This class is used to connect to a MQTT broker and publish and subscribe to topics.
//************************************************************************************************************************

BASEMQTT::BASEMQTT() : _interval(60000),
                       _interval_reconnect(5000),
                       _user_data{0},
                       _previousMillis(0),
                       _user_bytes_received(0),
                       _menuTopic("menuControl/menu"),
                       _infoTopic("user/info") {}

BASEMQTT::~BASEMQTT() {}

const char *getBrokerAddress()
{
#if ENABLE_MDNS_SUPPORT
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT " - Yes"))
    if (mDNSDiscovery::DiscovermDNSBroker())
    {
        Serial.println(F("[mDNS responder started] Setting up Broker..."));
        char *BROKER_ADDR = cfg.config.MQTTBroker; // IP address of the MQTT broker - change to your broker IP address or enable MDNS support
        return BROKER_ADDR;
    }
    else
    {
        Serial.println(F("[mDNS responder failed]"));
        char *BROKER_ADDR = MQTT_BROKER;
        return BROKER_ADDR;
    }
    return MQTT_BROKER;
#else
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT " - No"))
    return MQTT_BROKER;
#endif // ENABLE_MDNS_SUPPORT
}

// Handles messages arrived on subscribed topic(s)
void callback(char *topic, byte *payload, unsigned int length)
{
    std::string result = "";
    log_i("Message arrived on topic: [%s] ", topic);
    char buffer[length + 1];
    memcpy(buffer, payload, length);
    buffer[length] = '\0'; // Null-terminate the string
    result += buffer;
    log_i("payload is: [%s]", buffer);
    result += buffer;
    log_i("Message: [%s]", result.c_str());

    // Check if the message is for the current topic
    switch (basemqtt.s_callback_map[topic])
    {
    case basemqtt.Info:
        break;
    case basemqtt.Menu:
        break;
    case basemqtt.Relay:
        break;
#if ENABLE_PH_SUPPORT
    case basemqtt.Ph:
        log_i("Setting pH level to: [%s]", result.c_str());
        phsensor.parse_cmd_lookup(result.c_str());
        break;
#endif // ENABLE_PH_SUPPORT
    case basemqtt.Pump:
        switch (basemqtt.s_relay_control_map[result])
        {
        case basemqtt.On:
            log_i("Turning on the pump");
            Relay.RelayOnOff(pump.pump_data._pump_relay_pin, true);
            break;
        case basemqtt.Off:
            log_i("Turning off the pump");
            Relay.RelayOnOff(pump.pump_data._pump_relay_pin, false);
            break;
        case basemqtt.UnDefined:
            log_e("Undefined command");
            break;
        case basemqtt.End:
            //! Implement a method to turn off MQTT
            break;
        }
        break;
    default:
        break;
    }
}

bool BASEMQTT::begin()
{
    /* Initialize maps */
    s_callback_map["Undefined"] = UnDefinedCallback;
    s_callback_map[pump.pump_data._pumpTopic] = Pump;
    s_callback_map["Relay"] = Relay;
    s_callback_map[phsensor._phData._pHTopic] = Ph;
    s_callback_map[_infoTopic] = Info;
    s_callback_map[_menuTopic] = Menu;
    s_callback_map["End"] = EndCallback;

    s_relay_control_map["UNDEFINED"] = UnDefined;
    s_relay_control_map["ON"] = On;
    s_relay_control_map["OFF"] = Off;
    s_relay_control_map["End"] = End;

    log_i("Setting up MQTT...");

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
    log_i("Connection succeeded. Subscribing to the topic [%s]", phsensor._phData._pHTopic);
    mqttClient.subscribe(phsensor._phData._pHTopic);
#endif // ENABLE_PH_SUPPORT
    log_i("Subscribing to the topic [%s]", pump.pump_data._pumpTopic);
    mqttClient.subscribe(pump.pump_data._pumpTopic);

    log_i("Successfully subscribed to the topic.");
    /* _speakerTopic = SPEAKER_TOPIC;
    _waterlevelTopic = WATER_LEVEL_TOPIC; */
    return true;
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

void BASEMQTT::checkState()
{
    cfg.config.MQTTConnectedState = mqttClient.connected();

    cfg.config.MQTTConnectedState ? StateManager_MQTT.setState(ProgramStates::DeviceStates::MQTTState_e::MQTT_Connected) : StateManager_MQTT.setState(ProgramStates::DeviceStates::MQTTState_e::MQTT_Disconnected);
    log_i("MQTT client state is: %d", mqttClient.state());
}

void BASEMQTT::mqttReconnect()
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

void BASEMQTT::mqttLoop()
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
            if (Serial.available() > 0)
            {
                _user_bytes_received = Serial.readBytesUntil(13, _user_data, sizeof(_user_data));
            }

            if (_user_bytes_received)
            {
                std::string temp = _user_data;
                phsensor.parse_cmd_lookup(temp);
                _user_bytes_received = 0;
                memset(_user_data, 0, sizeof(_user_data));
            }

            log_i("Sending message to topic: %s", phsensor._phData._pHOutTopic);
            String timeStamp = networkntp.getTimeStamp();
            mqttClient.publish(phsensor._phData._pHOutTopic, timeStamp.c_str(), true);
        }
    }
}

BASEMQTT basemqtt;
