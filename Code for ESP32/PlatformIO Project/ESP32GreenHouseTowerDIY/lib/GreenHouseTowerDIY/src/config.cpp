#include "config.hpp"

// save last "timestamp" the config has been saved

Config::Config()
{
    last_config_change = false;
    last_config = 0;
    maxVoltage = 24;
    maxTemp = 120;
    doc_string = "";
}

Config::~Config()
{
    /* // Free the allocated memory
    if (config.hostname)
    {
        freeStr(&config.hostname);
    }
    if (config.MQTTBroker)
    {
        freeStr(&config.MQTTBroker);
    }
    if (config.MQTTPort)
    {
        freeStr(&config.MQTTPort);
    }
    if (config.MQTTUser)
    {
        freeStr(&config.MQTTUser);
    }
    if (config.MQTTPass)
    {
        freeStr(&config.MQTTPass);
    }
    if (config.MQTTTopic)
    {
        freeStr(&config.MQTTTopic);
    }
    if (config.MQTTSetTopic)
    {
        freeStr(&config.MQTTSetTopic);
    }
    if (config.MQTTDeviceName)
    {
        freeStr(&config.MQTTDeviceName);
    }
    if (config.NTPTIME)
    {
        freeStr(&config.NTPTIME);
    }
    if (config.NTPTIMEOFFSET)
    {
        freeStr(&config.NTPTIMEOFFSET);
    }
    if (config.WIFISSID)
    {
        freeStr(&config.WIFISSID);
    }
    if (config.WIFIPASS)
    {
        freeStr(&config.WIFIPASS);
    }
    if (config.MDNS)
    {
        freeStr(&config.MDNS);
    }
    if (config.DHCPCHECK)
    {
        freeStr(&config.DHCPCHECK);
    }
    if (config.configData)
    {
        freeStr(&config.configData);
    } */
}

void Config::CreateDefaultConfig()
{
    config.hostname = NULL;
    config.MQTTClientID = NULL;
    config.MQTTBroker = NULL;
    config.MQTTPort = 0; // Port to use for unsecured MQTT
    config.MQTTEnabled = false;     
    config.MQTTPort_Secure = 0; // port to use if Secured MQTT is enabled
    config.MQTTUser = NULL;
    config.MQTTPass = NULL;
    config.MQTTTopic = NULL;
    config.MQTTSetTopic = NULL;
    config.MQTTDeviceName = NULL;
    config.data_json = false;
    config.data_json_string = "";
    config.last_mqtt_connect_attempt = 0;
    config.last_mqtt_publish_attempt = 0;
    config.lastMillis = 0;
    config.IP = NULL;
    config.netmask = NULL;
    config.gateway = NULL;
    config.MQTTSecureState = false;
    config.lastMsg = 0;
    config.msg = NULL;
    config.value = 0;
    config.WIFISSID = NULL;
    config.WIFIPASS = NULL;
    config.MQTTConnectedState = false;
    config.NTPTIME = NULL;
    config.NTPTIMEOFFSET = NULL;
    config.MDNS = NULL;
    config.DHCPCHECK = NULL;
    config.numSensors = 0;
    config.cell_count_max = 0;

    for (int i = 0; i < 5; i++)
    {
        config.relays[i] = false;
    }

    config.stack_humidity = 0;
    config.stack_temp = 0;
    config.stack_voltage = 0;
    config.stack_current = 0;

    for (int i = 0; i < 10; i++)
    {
        config.cell_temp[i] = 0;
        config.cell_voltage[i] = 0;
    }
}

// Initialize SPIFFS
bool Config::initSPIFFS()
{
    if (!SPIFFS.begin(false))
    {
        log_e("An error has occurred while mounting SPIFFS");
        return false;
    }
    log_i("SPIFFS mounted successfully");
    return true;
}

// Read File from SPIFFS
String Config::readFile(fs::FS &fs, const char *path)
{
    log_i("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        log_e("[INFO]: Failed to open file for reading");
        return String();
    }

    String fileContent;
    while (file.available())
    {
        fileContent = file.readStringUntil('\n');
        break;
    }
    return fileContent;
}

// Write file to SPIFFS
void Config::writeFile(fs::FS &fs, const char *path, const char *message)
{
    log_i("[Writing File]: Writing file: %s\r\n", path);
    my_delay(0.1L);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        log_i("[Writing File]: failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        log_i("[Writing File]: file written");
    }
    else
    {
        log_i("[Writing File]: file write failed");
    }
}

bool Config::loadConfig()
{
    log_i("[Load config]: Loading Config File");

    // load the config file
    initSPIFFS();
    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile)
    {
        log_e("[Load config]: Failed to open config file");
        CreateDefaultConfig();
        return false;
    }

    size_t size = configFile.size();
    log_i("[Load config]: Config file size: %s", String(size).c_str());

    if (size > 1024)
    {
        log_e("[Load config]: Config file size is too large");
        CreateDefaultConfig();
        return false;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);
    // We don't use String here because ArduinoJson library requires the input
    // to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);
    log_i("[Load config]: Config file content: %s", buf.get());

    // Parse the buffer into an object
    StaticJsonDocument<1024> jsonBuffer;
    // Deserialize the JSON document
    auto error = deserializeJson(jsonBuffer, buf.get());
    if (error)
    {
        log_e("[Load config]: Failed to parse config file");
        log_e("[Load config]: deserializeJson() failed with code %s", error.c_str());
        CreateDefaultConfig();
        return false;
    }

    heapStr(&config.hostname, jsonBuffer["hostname"]);
    heapStr(&config.MQTTClientID, jsonBuffer["MQTTClientID"]);
    config.MQTTEnabled = jsonBuffer["MQTTEnabled"];
    config.MQTTPort = jsonBuffer["MQTTPort"];
    config.MQTTPort_Secure = jsonBuffer["MQTTPort_Secure"];
    heapStr(&config.MQTTUser, jsonBuffer["MQTTUser"]);
    heapStr(&config.MQTTPass, jsonBuffer["MQTTPass"]);
    heapStr(&config.MQTTTopic, jsonBuffer["MQTTTopic"]);
    heapStr(&config.MQTTSetTopic, jsonBuffer["MQTTSetTopic"]);
    heapStr(&config.MQTTDeviceName, jsonBuffer["MQTTDeviceName"]);
    config.last_mqtt_connect_attempt = jsonBuffer["last_mqtt_connect_attempt"];
    config.last_mqtt_publish_attempt = jsonBuffer["last_mqtt_publish_attempt"];
    config.lastMillis = jsonBuffer["lastMillis"];
    heapStr(&config.IP, jsonBuffer["IP"]);
    heapStr(&config.netmask, jsonBuffer["netmask"]);
    heapStr(&config.gateway, jsonBuffer["gateway"]);
    config.MQTTSecureState = jsonBuffer["MQTTSecureState"];
    heapStr(&config.MQTTBroker, jsonBuffer["MQTTBroker"]);
    config.lastMsg = jsonBuffer["lastMsg"];
    heapStr(&config.msg, jsonBuffer["msg"]);
    config.value = jsonBuffer["value"];
    heapStr(&config.WIFISSID, jsonBuffer["WIFISSID"]);
    heapStr(&config.WIFIPASS, jsonBuffer["WIFIPASS"]);
    config.MQTTConnectedState = jsonBuffer["MQTTConnectedState"];
    heapStr(&config.NTPTIME, jsonBuffer["NTPTIME"]);
    heapStr(&config.NTPTIMEOFFSET, jsonBuffer["NTPTIMEOFFSET"]);
    heapStr(&config.MDNS, jsonBuffer["MDNS"]);
    heapStr(&config.DHCPCHECK, jsonBuffer["DHCPCHECK"]);
    config.numSensors = jsonBuffer["Number_of_Sensors"];
    config.cell_count_max = jsonBuffer["Max_Cell_Count"];

    for (int i = 0; i < sizeof(config.relays); i++)
    {
        config.relays[i] = jsonBuffer["relays"][i];
    }

    return true;
}

// trigger a config write/commit
bool Config::setConfigChanged()
{
    last_config_change = true;
    saveConfig();
    log_i("[Set Config Changed]: Config save set to true");
    return true;
}

bool Config::saveConfig()
{
    // check if the data in config is different from the data in the file
    if (!last_config_change)
    {
        log_i("[Save Config Changes]: Config has not changed because it is the same as the file");
        return false;
    }

    log_i("[Save Config Changes]: Saving Config");

    // create a json file from the config struct and save it using SPIFFs
    log_i("[Save Config Changes]: Writing config");
    // create a json file from the config struct
    StaticJsonDocument<1024> jsonConfig;
    JsonObject json = jsonConfig.to<JsonObject>();

    // create a json file from the config struct
    json["hostname"] = config.hostname;
    json["MQTTClientID"] = config.MQTTClientID;
    json["MQTTEnabled"] = config.MQTTEnabled;
    json["MQTTPort"] = config.MQTTPort;
    json["MQTTPort_Secure"] = config.MQTTPort_Secure;
    json["MQTTUser"] = config.MQTTUser;
    json["MQTTPass"] = config.MQTTPass;
    json["MQTTTopic"] = config.MQTTTopic;
    json["MQTTSetTopic"] = config.MQTTSetTopic;
    json["MQTTDeviceName"] = config.MQTTDeviceName;
    json["last_mqtt_connect_attempt"] = config.last_mqtt_connect_attempt;
    json["last_mqtt_publish_attempt"] = config.last_mqtt_publish_attempt;
    json["lastMillis"] = config.lastMillis;
    json["IP"] = config.IP;
    json["netmask"] = config.netmask;
    json["gateway"] = config.gateway;
    json["MQTTSecureState"] = config.MQTTSecureState;
    json["MQTTBroker"] = config.MQTTBroker;
    json["lastMsg"] = config.lastMsg;
    json["msg"] = config.msg;
    json["value"] = config.value;
    json["WIFISSID"] = config.WIFISSID;
    json["WIFIPASS"] = config.WIFIPASS;
    json["MQTTConnectedState"] = config.MQTTConnectedState;
    json["NTPTIME"] = config.NTPTIME;
    json["NTPTIMEOFFSET"] = config.NTPTIMEOFFSET;
    json["MDNS"] = config.MDNS;
    json["DHCPCHECK"] = config.DHCPCHECK;
    json["Number_of_Sensors"] = config.numSensors;
    json["Max_Cell_Count"] = config.cell_count_max;

    // Relays
    JsonArray Relays = json.createNestedArray("HMS_Relays_State");
    for (int i = 0; i < sizeof(config.relays) / sizeof(config.relays[0]); i++)
    {
        Relays.add(config.relays[i]);
    }

    // Set the values in the document
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
        log_e("[Save Config Changes]: Failed to open config file for writing");
        return false;
    }
    if (serializeJson(json, configFile) == 0)
    {
        log_e("[Save Config Changes]: Failed to write to file");
        return false;
    }

    configFile.print("\r\n");
    configFile.close();
    // end save
    log_i("[Save Config Changes]: Config written");
    last_config_change = false;

    return true;
}

bool Config::updateCurrentData()
{
    // check if the data in config is different from the data in the file
    if (!last_config_change)
    {
        log_i("[Update Current Data]: Config has not changed because it is the same as the file");
        return false;
    }
    log_i("[Update Current Data]: Updating Config");
    // call to save config if config has changed
    saveConfig();
    log_i("[INFO - FREE HEAP SIZE]: Heap: %d", ESP.getFreeHeap());
    return true;
}

// overwrite all config settings with "0"
void Config::resetConfig()
{
    CreateDefaultConfig();
    setConfigChanged();
}

bool Config::isValidHostname(char *hostname_to_check, long size)
{
    for (int i = 0; i < size; i++)
    {
        if (hostname_to_check[i] == '-' || hostname_to_check[i] == '.')
            continue;
        else if (hostname_to_check[i] >= '0' && hostname_to_check[i] <= '9')
            continue;
        else if (hostname_to_check[i] >= 'A' && hostname_to_check[i] <= 'Z')
            continue;
        else if (hostname_to_check[i] >= 'a' && hostname_to_check[i] <= 'z')
            continue;
        else if (hostname_to_check[i] == 0 && i > 0)
            break;

        return false;
    }

    return true;
}

// parse and set a new hostname to config
void Config::setHostname(String new_hostname)
{
    int j = 0;
    for (unsigned int i = 0; i < new_hostname.length() && i < sizeof(config.hostname); i++)
    {
        if (new_hostname.charAt(i) == '-' or
            (new_hostname.charAt(i) >= '0' && new_hostname.charAt(i) <= '9') or
            (new_hostname.charAt(i) >= 'A' && new_hostname.charAt(i) <= 'Z') or
            (new_hostname.charAt(i) >= 'a' && new_hostname.charAt(i) <= 'z'))
        {

            config.hostname[j] = new_hostname.charAt(i);
            j++;
        }
    }
    config.hostname[j] = '\0';
    setConfigChanged();
}

// we can't assing wifiManager.resetSettings(); to reset, somehow it gets called straight away.
void Config::setWiFiConf(String ssid, String password)
{
    /* #if defined(ESP32)
        if (WiFiGenericClass::getMode() != WIFI_MODE_NULL)
        {

            wifi_config_t conf;
            esp_wifi_get_config(WIFI_IF_STA, &conf);

            memset(conf.sta.ssid, 0, sizeof(conf.sta.ssid));
            for (int i = 0; i < ssid.length() && i < sizeof(conf.sta.ssid); i++)
                conf.sta.ssid[i] = ssid.charAt(i);

            memset(conf.sta.password, 0, sizeof(conf.sta.password));
            for (int i = 0; i < password.length() && i < sizeof(conf.sta.password); i++)
                conf.sta.password[i] = password.charAt(i);

            esp_wifi_set_config(WIFI_IF_STA, &conf);
        }
    #endif */
}

Config cfg;
