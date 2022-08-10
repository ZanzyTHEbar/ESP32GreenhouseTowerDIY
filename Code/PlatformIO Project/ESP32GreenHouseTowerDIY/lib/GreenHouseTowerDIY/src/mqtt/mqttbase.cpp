#include "mqttbase.hpp"

std::unordered_map<std::string, XMqttBaseClass::RelayEnum> XMqttBaseClass::s_relay_control_map(0);
std::unordered_map<std::string, XMqttBaseClass::PumpEnum> XMqttBaseClass::s_pump_control_map(0);
std::unordered_map<std::string, XMqttBaseClass::CallbackEnum> XMqttBaseClass::s_callback_map(0);

XMqttBaseClass::XMqttBaseClass() : _menuTopic("menuControl/menu"), _infoTopic("user/info") {}

XMqttBaseClass::~XMqttBaseClass() {}

bool XMqttBaseClass::begin()
{
    /* Initialize maps */
    s_callback_map.emplace("Undefined", UnDefinedCallback);
    s_callback_map[pump.pump_data._pumpTopic] = Pump;
    s_callback_map["Relay"] = Relays;
#if ENABLE_PH_SUPPORT
    if (phsensor.ph_data.find("id") != phsensor.ph_data.end())
    {
        const PHSENSOR::ph_Data_t &phData = phsensor.ph_data.at("id");
        s_callback_map[phData._pHTopic] = Ph;
    }
#endif // ENABLE_PH_SUPPORT
    s_callback_map[_infoTopic] = Info;
    s_callback_map[_menuTopic] = Menu;
    s_callback_map["End"] = EndCallback;

    s_relay_control_map["UNDEFINED"] = UnDefined;
    s_relay_control_map["ON"] = On;
    s_relay_control_map["OFF"] = Off;
    s_relay_control_map["End"] = End;

    log_i("Setting up MQTT...");
    return true;
}

/**
 * @brief Check if the current hostname is the same as the one in the config file
 * Call in the Setup BEFORE the WiFi.begin()
 * @param None
 * @return None
 */
void XMqttBaseClass::loadMQTTConfig()
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

const char *XMqttBaseClass::getBrokerAddress()
{
#if ENABLE_MDNS_SUPPORT
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT " - Yes"))
    if (mDNSDiscovery::DiscovermDNSBroker())
    {
        Serial.println(F("[mDNS responder started] Setting up Broker..."));
        const char *BROKER_ADDR = cfg.config.MQTTBroker; // IP address of the MQTT broker - change to your broker IP address or enable MDNS support
        return BROKER_ADDR;
    }
    else
    {
        Serial.println(F("[mDNS responder failed]"));
        const char *BROKER_ADDR = MQTT_BROKER;
        return BROKER_ADDR;
    }

#else
#pragma message(Feature "mDNS Enabled: " STR(ENABLE_MDNS_SUPPORT " - No"))
    return MQTT_BROKER;
#endif // ENABLE_MDNS_SUPPORT
}

void XMqttBaseClass::callback(const char *topic, byte *payload, unsigned int length)
{
    std::string result = "";
    log_i("Message arrived on topic: [%s] ", topic);
    char buffer[length + 1];
    memcpy(buffer, payload, length);
    buffer[length] = '\0'; // Null-terminate the string
    log_i("payload is: [%s]", buffer);
    result += buffer;
    log_i("Message: [%s]", result.c_str());

    // Check if the message is for the current topic
    switch (s_callback_map[topic])
    {
    case Info:
        break;
    case Menu:
        break;
    case Relays:
        break;
#if ENABLE_PH_SUPPORT
    case Ph:
        log_i("Setting pH level to: [%s]", buffer);
        phsensor.parse_cmd_lookup(result);
        break;
#endif // ENABLE_PH_SUPPORT
    case Pump:
        switch (s_relay_control_map[result])
        {
        case On:
            log_i("Turning on the pump");
            Relay.RelayOnOff(pump.pump_data._pump_relay_pin, true);
            break;
        case Off:
            log_i("Turning off the pump");
            Relay.RelayOnOff(pump.pump_data._pump_relay_pin, false);
            break;
        case UnDefined:
            log_e("Undefined command");
            break;
        case End:
            //! Implement a method to turn off MQTT
            break;
        }
        break;
    default:
        break;
    }
}

void XMqttBaseClass::callback(const char *topic, const uint8_t *payload, uint16_t length)
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
    switch (s_callback_map[topic])
    {
    case Info:
        break;
    case Menu:
        break;
    case Relays:
        break;
#if ENABLE_PH_SUPPORT
    case Ph:
        log_i("Setting pH level to: [%s]", result.c_str());
        phsensor.parse_cmd_lookup(result);
        break;
#endif // ENABLE_PH_SUPPORT
    case Pump:
        switch (s_relay_control_map[result])
        {
        case On:
            log_i("Turning on the pump");
            Relay.RelayOnOff(pump.pump_data._pump_relay_pin, true);
            break;
        case Off:
            log_i("Turning off the pump");
            Relay.RelayOnOff(pump.pump_data._pump_relay_pin, false);
            break;
        case UnDefined:
            log_e("Undefined command");
            break;
        case End:
            //! Implement a method to turn off MQTT
            break;
        }
        break;
    default:
        break;
    }
}

XMqttBaseClass baseMQTT;