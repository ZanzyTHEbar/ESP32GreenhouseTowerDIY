#include "network.hpp"
/**
 * WiFi Events
 *0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
 *1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
 *2  SYSTEM_EVENT_STA_START                < ESP32 station start
 *3  SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
 *4  SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
 *5  SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
 *6  SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
 *7  SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
 *8  SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
 *9  SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
 *10 SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
 *11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
 *12 SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
 *13 SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
 *14 SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
 *15 SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
 *16 SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
 *17 SYSTEM_EVENT_AP_STAIPASSIGNED         < ESP32 soft-AP assign an IP to a connected station
 *18 SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
 *19 SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
 *20 SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
 *21 SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
 *22 SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
 *23 SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
 *24 SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
 *25 SYSTEM_EVENT_MAX

 WiFi.status() return values:
    *0  WL_IDLE_STATUS                       < ESP32 station is in idle state
    *1  WL_NO_SSID_AVAIL                     < ESP32 station cannot find any SSID in scan results
    *2  WL_SCAN_COMPLETED                    < ESP32 station has finished the scan
    *3  WL_CONNECTED                         < ESP32 station has a connection to an AP
    *4  WL_CONNECT_FAILED                    < ESP32 station failed to connect to an AP
    *5  WL_CONNECTION_LOST                   < ESP32 station has lost connection to an AP
    *6  WL_DISCONNECTED                      < ESP32 station is in disconnected state
 */

//*===========================================================================
//* BUTT Lib - Network
//*===========================================================================

Network::Network(Config *cfg, StateManager<WiFiState_e> *stateManager) : wifi_conf(new wifi_config_t),
                                                                         stateManager(stateManager),
                                                                         _interval(1000),
                                                                         _previousMillis(0),
                                                                         cfg(cfg) {}

Network::~Network() {}

void Network::setCallback(callback_t callback) { mCallback = callback; }

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Network::setup()
{
  unsigned int ssid_length = sizeof(wifi_conf->sta.ssid);
  unsigned int password_length = sizeof(wifi_conf->sta.password);

  char ssid[ssid_length + 1];
  char password[ssid_length + 1];
  memcpy(ssid, wifi_conf->sta.ssid, ssid_length);
  memcpy(password, wifi_conf->sta.password, password_length);

  ssid[ssid_length] = '\0';         // Null-terminate the string
  password[password_length] = '\0'; // Null-terminate the string

  cfg->CreateDefaultConfig();
  if (!cfg->loadConfig())
  {
    log_i("[INFO]: Failed to load config\n");
    return false;
  }

  log_i("[INFO]: Loaded config\n");
  // Load values saved in SPIFFS
  // SSID = cfg->config.WIFISSID;
  // PASS = cfg->config.WIFIPASS;

  if (!PRODUCTION)
  {
    // print it on the serial monitor
    log_i("%s\n", password);
  }

  if (ssid[0] == '\0')
  {
    log_i("[INFO]: No SSID or password has been set.\n");
    log_i("[INFO]: Please configure the Wifi Manager by scanning the QR code on your device.\r\n");
    return false;
  }

  if (password[0] == '\0')
  {
    log_i("[INFO]: No Password has been set.\n");
  }

  log_i("[INFO]: Configured SSID: %s\r\n", ssid);

  // Set your Gateway IP address
  /* IPAddress localIP;
  IPAddress gateway;
  IPAddress subnet;

  WiFi.mode(WIFI_STA);
  localIP.fromString(WiFi.localIP().toString());
  gateway.fromString(WiFi.gatewayIP().toString());
  subnet.fromString(WiFi.subnetMask().toString()); */

  if (!WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE))
  {
    log_e("[INFO]: STA Failed to configure\n");
    return false;
  }

  WiFi.setHostname(cfg->config.hostname); // define hostname

  WiFi.begin(ssid, password); // connect to WiFi network

  unsigned long currentMillis = millis();
  _previousMillis = currentMillis;

  while (WiFi.status() != WL_CONNECTED)
  {
    stateManager->setState(ProgramStates::DeviceStates::WiFiState_e::WiFiState_Connecting);
    currentMillis = millis();
    if (currentMillis - _previousMillis >= _interval)
    {
      log_i("[INFO]: WiFi connection timed out.\n");
      stateManager->setState(ProgramStates::DeviceStates::WiFiState_e::WiFiState_Error);
      return false;
    }
  }

  log_i("[INFO]: Connected to WiFi.\n");
  log_i("IP address: %s\n", WiFi.localIP().toString().c_str());
  stateManager->setState(ProgramStates::DeviceStates::WiFiState_e::WiFiState_Connected);
  return true;
}

/**
 * @brief Setup the network stack and routes - fail to AP mode if STA fails
 *
 */
void Network::begin()
{
  if (setup())
  {
    mCallback();
    log_i("ESP32GreenHouseDIY HMS server started\n");
  }
  else
  {
    // TODO: Route for root to  "Please Scan QR code"
    // TODO: There should be a reset mode that will reset the device to factory settings and restart the device.
    // TODO: Should be a physical reset button on the PCB itself - not a touch button - hold for 5 seconds to reset. Flash LED to indicate reset per second.
    // Connect to Wi-Fi Network with SSID and password

    log_i("[INFO]: Setting Access Point...\n");

    log_i("[INFO]: Configuring access point...\n");
    WiFi.mode(WIFI_AP);

    // You can remove the password parameter if you want the AP to be open.
    log_i("Wifi Connection Failed. \r\nStarting AP. \r\nAP IP address: ");
    IPAddress IP = WiFi.softAPIP();
    log_i("[INFO]: AP IP address: %s.\r\n", IP.toString().c_str());

    if (!PRODUCTION)
    {
      WiFi.softAP(DEFAULT_HOSTNAME, NULL, 10, 0, 2); // AP mode without password
    }
    else
    {
      WiFi.softAP(WIFI_SSID, WIFI_PASS, 10, 0, 3); // AP mode with password
    }
    WiFi.setTxPower(WIFI_POWER_11dBm);

    mCallback(); // call the network routes function to setup the routes
  }
}

// ######################## server functions #########################

/******************************************************************************
 * Function: Check Network Connection Loop
 * Description: This function checks the Network connection and reconnects if necessary - is called in the loop() function every 5 seconds
 * Parameters: None
 * Return: None
 ******************************************************************************/
void Network::CheckNetworkLoop()
{
  // run current function every 5 seconds
  stateManager->setState((WiFi.status() != WL_CONNECTED) ? WiFiState_e::WiFiState_Disconnected : WiFiState_e::WiFiState_Connected);
  log_i("Wifi is %s ", ((WiFi.status() != WL_CONNECTED) ? " connected" : "not connected"));
}

void Network::CheckConnectionLoop_Active()
{
  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting
  if (stateManager->getCurrentState() == WiFiState_e::WiFiState_Disconnected && (currentMillis - _previousMillis >= _interval))
  {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    // WiFi.disconnect(); // disconnect from previous Access Point's - if connected
    WiFi.reconnect();
    _previousMillis = currentMillis;
  }
}

void Network::SetupServer()
{
  log_i("\n\nSystem Information:\n\n");
  log_i("PLatformI0 compile time: %s (%s)\n", __DATE__, __TIME__);
  log_i("PLatformI0 Unix compile time: %d\n", COMPILE_UNIX_TIME);
  log_i("Project directory: %s\n", PROJECT_PATH);
  log_i("Version: %d\n", VERSION);
  log_i("Heap: %d\n", ESP.getFreeHeap());
  log_i("SDK: %s\n", ESP.getSdkVersion());
  log_i("MAC address: %s\n", WiFi.macAddress().c_str());
  log_i("CPU Speed: %dMHz\n", ESP.getCpuFreqMHz());
  log_i("Flash Size: %dKB\n", ESP.getFlashChipSize());
  log_i("[INFO]: System Information Sent\n\n");

  // FS debug information
  // THIS NEEDS TO BE PAST THE WIFI SETUP!! OTHERWISE WIFI SETUP WILL BE DELAYED
  log_i("SPIFFS contents:\n");
#ifdef ESP32
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file)
  {
    log_i("FS File: %s, size: %i\n", file.name(), file.size());
    file = root.openNextFile();
  }
  log_i("\n");
  unsigned int totalBytes = SPIFFS.totalBytes();
  unsigned int usedBytes = SPIFFS.usedBytes();
  if (usedBytes == 0)
  {
    log_i("NO WEB SERVER FILES PRESENT: \n");
  }
  log_i("FS Size: %iKB, used: %iKB, %0.2f%%\n",
        totalBytes, usedBytes,
        (float)100 / totalBytes * usedBytes);
  log_i("\n");
#endif
}

// ############## functions to update current server settings ###################
/**
 * @brief Check if the current hostname is the same as the one in the config file
 * Call in the Setup BEFORE the WiFi.begin()
 * @param None
 * @return None
 */
void Network::CheckIfValidHostname()
{
  log_i("Checking if hostname is set and valid");
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  size_t size = sizeof(cfg->config.hostname);
  if (!cfg->isValidHostname(cfg->config.hostname, size - 1))
  {
    strncpy(cfg->config.hostname, DEFAULT_HOSTNAME, size - 1);
    cfg->setConfigChanged();
  }
  WiFi.setHostname(cfg->config.hostname); // define hostname
  log_i("Loaded config: hostname %s", cfg->config.hostname);
}

// we can't assign wifiManager.resetSettings(); to reset, somehow it gets called straight away.
void Network::setWiFiConf(const char *value, uint8_t *location, wifi_config_t *conf)
{
  assert(conf != nullptr);
#if defined(ESP32)
  if (WiFiGenericClass::getMode() != WIFI_MODE_NULL)
  {
    esp_wifi_get_config(WIFI_IF_STA, conf);

    memset(location, 0, sizeof(location));
    for (int i = 0; i < sizeof(value) / sizeof(value[0]) && i < sizeof(location); i++)
      location[i] = value[i];

    esp_wifi_set_config(WIFI_IF_STA, conf);
  }
#endif
  // EOF