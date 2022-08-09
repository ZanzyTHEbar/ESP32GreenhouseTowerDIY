#include "base.hpp"

BaseMQTT::BaseMQTT() : _menuTopic("menuControl/menu"),
                       _infoTopic("user/info") {}

BaseMQTT::~BaseMQTT() {}

bool BaseMQTT::begin()
{
    /* Initialize maps */
    s_callback_map["Undefined"] = UnDefinedCallback;
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
void BaseMQTT::loadMQTTConfig()
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

const char *BaseMQTT::getBrokerAddress()
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