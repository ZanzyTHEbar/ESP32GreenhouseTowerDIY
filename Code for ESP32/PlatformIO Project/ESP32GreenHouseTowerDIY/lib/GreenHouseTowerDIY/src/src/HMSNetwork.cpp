#include "HMSnetwork.hpp"

AsyncWebServer server(80);

WiFiClient espClient;

const size_t MAX_FILESIZE = 1024 * 1024 * 2; // 2MB
const char *mdnsPath = "/mdns.txt";
const char *dhcpcheckPath = "/dhcpcheck.txt";
const char *ssidPath = "/ssid.txt";
const char *passPath = "/pass.txt";
const char *ntptimePath = "/ntptime.txt";
const char *ntptimeoffsetPath = "/ntptimeoffset.txt";

const char *HTTP_USERNAME = "admin";
const char *HTTP_PASSWORD = "admin";

// char* create_mDNS_hostname = StringtoChar(MQTTCreateHostName(MQTT_HOSTNAME, ".local"));

String ledState;

// Timer variables
unsigned long previousMillis = 0;
const long interval = 30000; // interval to wait for Wi-Fi connection (milliseconds)

HMSnetwork::HMSnetwork()
{
    // constructor
    log_i("[INFO]: HMSnetwork::HMSnetwork()\n");
    log_i("[INFO]: Creating network object\n");
}

HMSnetwork::~HMSnetwork()
{
    // destructor
    log_i("[INFO]: HMSnetwork::~HMSnetwork()\n");
    log_i("[INFO]: Destroying network object\n");
}

/* format bytes as KB, MB or GB string */
String humanReadableSize(const size_t bytes)
{
    if (bytes < 1024)
        return String(bytes) + " B";
    else if (bytes < (1024 * 1024))
        return String(bytes / 1024.0) + " KB";
    else if (bytes < (1024 * 1024 * 1024))
        return String(bytes / 1024.0 / 1024.0) + " MB";
    else
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

void notFound(AsyncWebServerRequest *request)
{
    log_i("NOT_FOUND: ");
    if (request->method() == HTTP_GET)
        log_i("GET");
    else if (request->method() == HTTP_POST)
        log_i("POST");
    else if (request->method() == HTTP_DELETE)
        log_i("DELETE");
    else if (request->method() == HTTP_PUT)
        log_i("PUT");
    else if (request->method() == HTTP_PATCH)
        log_i("PATCH");
    else if (request->method() == HTTP_HEAD)
        log_i("HEAD");
    else if (request->method() == HTTP_OPTIONS)
        log_i("OPTIONS");
    else
        log_i("UNKNOWN");
    log_i(" http://%s%s\n", request->host().c_str(), request->url().c_str());
    request->send(404, "text/plain", "Not found.");
}

std::vector<String> splitStringToVector(String msg)
{
    std::vector<String> subStrings;
    int j = 0;
    for (int i = 0; i < msg.length(); i++)
    {
        if (msg.charAt(i) == '.')
        {
            subStrings.push_back(msg.substring(j, i));
            j = i + 1;
        }
    }
    subStrings.push_back(msg.substring(j, msg.length())); // to grab the last value of the string
    for (int x = 0; x < subStrings.size(); x++)
    {
        log_i("[INFO]: %s\n", subStrings[x].c_str());
    }
    return subStrings;
}

bool HMSnetwork::SetupNetworkStack()
{
    String SSID;
    String PASS;
    String ntptime;
    String ntptimeoffset;
    String mdns;
    String dhcpcheck;
    String IP;
    String _gateway_;
    String _subnet_;

    cfg.CreateDefaultConfig();
    if (!cfg.loadConfig())
    {
        log_i("[INFO]: Failed to load config\n");
    }
    else
    {
        log_i("[INFO]: Loaded config\n");
        // Load values saved in SPIFFS
        SSID = cfg.config.WIFISSID;
        PASS = cfg.config.WIFIPASS;
        ntptime = cfg.config.NTPTIME;
        ntptimeoffset = cfg.config.NTPTIMEOFFSET;
        mdns = cfg.config.MDNS;
        dhcpcheck = cfg.config.DHCPCHECK;
        IP = cfg.config.IP;
        _gateway_ = cfg.config.gateway;
        _subnet_ = cfg.config.netmask;

        if (!PRODUCTION)
        {
            // print it on the serial monitor
            log_i("%s\n", PASS.c_str());
        }

        log_i("%s\n", mdns.c_str());
        log_i("%s\n", dhcpcheck.c_str());

        if (SSID[0] == '\0' || PASS[0] == '\0')
        {
            log_i("[INFO]: No SSID or password has been set.\n");
            log_i("[INFO]: Please configure the Wifi Manager by scanning the QR code on your device.\r\n");
            return false;
        }
        else
        {
            log_i("[INFO]: Configured SSID: %s\r\n", SSID.c_str());

            // Set your Gateway IP address
            IPAddress localIP;
            IPAddress gateway;
            IPAddress subnet;

            IPAddress _ip;
            IPAddress _gateway;
            IPAddress _subnet;

            WiFi.mode(WIFI_STA);
            localIP.fromString(WiFi.localIP().toString());
            gateway.fromString(WiFi.gatewayIP().toString());
            subnet.fromString(WiFi.subnetMask().toString());

            _ip.fromString(IP);
            _gateway.fromString(_gateway_);
            _subnet.fromString(_subnet_);

            if (dhcpcheck == "on")
            {
                log_i("[INFO]: DHCP is on\n");
                if (!WiFi.config(localIP, gateway, subnet))
                {
                    log_e("[INFO]: STA Failed to configure\n");
                    return false;
                }
            }
            else
            {
                log_i("[INFO]: DHCP Check is off\n");
                log_i("[INFO]: Using custom configuration\n");
                if (!WiFi.config(_ip, _gateway, _subnet))
                {
                    log_e("[INFO]: STA Failed to configure.\n");
                    return false;
                }
            }
            WiFi.begin(cfg.config.WIFISSID, cfg.config.WIFIPASS);

            unsigned long currentMillis = millis();
            previousMillis = currentMillis;

            while (WiFi.status() != WL_CONNECTED)
            {
                currentMillis = millis();
                if (currentMillis - previousMillis >= interval)
                {
                    log_i("[INFO]: WiFi connection timed out.\n");
                    return false;
                }
            }

            log_i("[INFO]: Connected to WiFi.\n");
            log_i("IP address: %s\n", WiFi.localIP().toString().c_str());
            return true;
        }
    }
    return false;
}

void HMSnetwork::SetupWebServer()
{
    if (SetupNetworkStack())
    {
        networkRoutes();
        log_i("HBAT HMS server started\n");
    }
    else
    {
        // TODO: Route for root to  "Please Scan QR code" - Route for Wifi Manager /HBAT_HMS wifi page
        // TODO: There should be a reset mode that will reset the device to factory settings and restart the device.
        // TODO: Should be a physical reset button on the PCB itself - not a touch button - hold for 5 seconds to reset. Flash LED to indicate reset per second.
        // Connect to Wi-Fi HMSnetwork with SSID and password

        log_i("[INFO]: Setting Access Point...\n");

        char *macAddr = StringtoChar(WiFi.macAddress());

        unsigned char *hash = MD5::make_hash(macAddr);

        // generate the digest (hex encoding) of our hash
        char *md5str = MD5::make_digest(hash, 16);

        if (!PRODUCTION)
        {
            // print it on the serial monitor
            log_i("[INFO]: MD5 HASH of MAC ADDRESS: %s\n", md5str);
        }

        log_i("[INFO]: Configuring access point...\n");
        WiFi.mode(WIFI_AP);
        WiFi.setTxPower(WIFI_POWER_11dBm);

        // You can remove the password parameter if you want the AP to be open.
        log_i("Wifi Connection Failed. \r\nStarting AP. \r\nAP IP address: ");
        IPAddress IP = WiFi.softAPIP();
        log_i("[INFO]: AP IP address: %s.\r\n", IP.toString().c_str());

        if (!PRODUCTION)
        {
            WiFi.softAP(WIFI_SSID, WIFI_PASS, 10, 0, 3); // AP mode with password
        }
        else
        {
            WiFi.softAP("HBAT_HMS", md5str, 10, 1, 2); // MAC address is used as password for the AP - Unique to each device - MD5 hash of MAC address
        }

        // Give the Memory back to the System
        free(md5str);

        // free dynamically allocated 16 byte hash from make_hash()
        free(hash);

        networkRoutes(); // call the network routes function to setup the routes
    }
}

void HMSnetwork::networkRoutes()
{
    static const char *MIMETYPE_HTML{"text/html"};
    /* static const char *MIMETYPE_CSS{"text/css"}; */
    /* static const char *MIMETYPE_JS{"application/javascript"}; */
    /* static const char *MIMETYPE_PNG{"image/png"}; */
    /* static const char *MIMETYPE_JPG{"image/jpeg"}; */
    /* static const char *MIMETYPE_ICO{"image/x-icon"}; */
    static const char *MIMETYPE_JSON{"application/json"};

    // Web Server Root URL
    server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/frontend.html", MIMETYPE_HTML); });

    server.serveStatic("/", SPIFFS, "/");

    server.on("/wifimanager", HTTP_GET, [&](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/wifimanager.html", MIMETYPE_HTML); });

    server.serveStatic("/wifimanager", SPIFFS, "/");

    // preflight cors check
    server.on("/", HTTP_OPTIONS, [&](AsyncWebServerRequest *request)
              {
        AsyncWebServerResponse* response = request->beginResponse(204);
        response->addHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization, FileSize");
        response->addHeader("Access-Control-Allow-Credentials", "true");
        request->send(response); });

    server.on("/upload", HTTP_GET, [&](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/upload.html", MIMETYPE_HTML); });

    server.serveStatic("/upload.html", SPIFFS, "/");

    server.on(
        "/api/upload", HTTP_POST, [&](AsyncWebServerRequest *request)
        {
        if (request->authenticate(HTTP_USERNAME, HTTP_PASSWORD))
            request->send(200);
        else {
            request->send(401);
            request->client()->close();
        } },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            if (!request->authenticate(HTTP_USERNAME, HTTP_PASSWORD))
            {
                request->send(401);
                request->client()->close();
                return;
            }

            static unsigned long startTimer;
            if (!index)
            {
                startTimer = millis();
                const char *FILESIZE_HEADER{"FileSize"};

                log_i("UPLOAD: Receiving: '%s'\n", filename.c_str());

                if (!request->hasHeader(FILESIZE_HEADER))
                {
                    request->send(400, MIMETYPE_HTML, "No filesize header present!");
                    request->client()->close();
                    log_e("UPLOAD: Aborted upload because missing filesize header.\n");
                    return;
                }

                log_i("UPLOAD: fileSize: %s\n", request->header(FILESIZE_HEADER).c_str());

                if (request->header(FILESIZE_HEADER).toInt() >= MAX_FILESIZE)
                {
                    request->send(400, MIMETYPE_HTML,
                                  "Too large. (" + humanReadableSize(request->header(FILESIZE_HEADER).toInt()) +
                                      ") Max size is " + humanReadableSize(MAX_FILESIZE) + ".");

                    request->client()->close();
                    log_e("UPLOAD: Aborted upload because filesize limit.\n");
                    return;
                }
            }

            if (final)
                log_i("UPLOAD: Done. Received %i bytes in %.2fs which is %.2f kB/s.\n",
                      index + len,
                      (millis() - startTimer) / 1000.0,
                      1.0 * (index + len) / (millis() - startTimer));
        });

    server.on("/wifimanager", HTTP_POST, [&](AsyncWebServerRequest *request)
              {
                int params = request->params();
                for (int i = 0; i < params; i++)
                {
                    AsyncWebParameter *p = request->getParam(i);
                    if (p->isPost())
                    {
                        // HTTP POST ssid value
                        if (p->name() == "ssid" && !p->value().isEmpty())
                        {
                            String _ssid;
                            _ssid = p->value().c_str();
                            log_i("SSID set to: %s\n", _ssid.c_str());

                            heapStr(&cfg.config.WIFISSID, _ssid.c_str());
                            my_delay(0.1L);
                        }

                        // HTTP POST pass value
                        if (p->name() == "password" && !p->value().isEmpty())
                        {
                            String _password;
                            _password = p->value().c_str();

                            heapStr(&cfg.config.WIFIPASS, _password.c_str());
                            my_delay(0.1L);
                        }

                        // HTTP DHCP VALUES
                        if (p->name() == "dhcp" && !p->value().isEmpty())
                        {
                            String _dhcp;
                            _dhcp = p->value().c_str();

                            heapStr(&cfg.config.DHCPCHECK, _dhcp.c_str());
                            my_delay(0.1L);
                        }
                        if (String(cfg.config.DHCPCHECK) != "on")
                        {
                            if (p->name() == "ip" && !p->value().isEmpty())
                            {
                                String _ip;
                                _ip = p->value().c_str();

                                heapStr(&cfg.config.IP, _ip.c_str());
                                my_delay(0.1L);
                            }
                            if (p->name() == "subnet" && !p->value().isEmpty())
                            {
                                String _netmask;
                                _netmask = p->value().c_str();

                                heapStr(&cfg.config.netmask, _netmask.c_str());
                                my_delay(0.1L);
                            }
                            if (p->name() == "gateway" && !p->value().isEmpty())
                            {
                                String _gateway;
                                _gateway = p->value().c_str();

                                heapStr(&cfg.config.gateway, _gateway.c_str());
                                my_delay(0.1L);
                            }
                        }

                        // MDNS VALUES
                        if (p->name() == "mdns" && !p->value().isEmpty())
                        {
                            String _mdns;
                            _mdns = p->value().c_str();
                            heapStr(&cfg.config.MDNS, _mdns.c_str());
                            my_delay(0.1L);
                        }
                        log_i("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
                    }
                }
                cfg.setConfigChanged();
                request->send(200, MIMETYPE_JSON, "Done. ESP will restart and connect to your router");
                my_delay(0.03L);
                ESP.restart(); });

    server.on("/mqttUpdate", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                int params = request->params();
                for(int i=0;i<params;i++){
                    AsyncWebParameter* p = request->getParam(i);
                    if (p->name() == "mqttIP") {
                        String broker;
                        broker = p->value().c_str();
                        log_i("SSID set to: %s\n", broker.c_str());
                        heapStr(&cfg.config.MQTTBroker, broker.c_str());
                        my_delay(0.1L);
                    }
                    if (p->name() == "mqttName") {
                        String user; 
                        user = p->value().c_str();
                        log_i("SSID set to: %s\n", user.c_str());
                        heapStr(&cfg.config.MQTTUser, user.c_str());
                        my_delay(0.1L);
                    }
                    if (p->name() == "mqttPass") {
                        String pass; 
                        pass = p->value().c_str();
                        heapStr(&cfg.config.MQTTPass, pass.c_str());
                        my_delay(0.1L);
                    }
                    log_i("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                }
                cfg.setConfigChanged();
                request->send(200, MIMETYPE_JSON, "Done. ESP will now connect to your broker");
                my_delay(0.03L);
                ESP.restart(); });

    // Route to set GPIO state to LOW
    server.on("/toggle", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                    int params = request->params();
                    for(int i=0;i<params;i++)
                    {
                        AsyncWebParameter* p = request->getParam(i);
                            // HTTP POST Relay Value
                        if (p->name() == "pin") 
                        {
                            String relay = p->value().c_str();
                            log_i("switching state of pin: %s\n", relay.c_str());
                            cfg.config.relays[relay.toInt()] = (cfg.config.relays[relay.toInt()] == true) ? false : true;
                        }
                        log_i("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                    }
                    request->send(200, MIMETYPE_JSON, "toggled"); });

    server.on("/mqttEnable", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                int params = request->params();
                for(int i=0;i<params;i++){
                    AsyncWebParameter* p = request->getParam(i);
                        // HTTP POST Relay Value
                    if (p->name() == "mqttEnableState") {
                        String state = p->value().c_str();
                        log_i("switching state of pin: %s\n", state.c_str());
                        //split variable on "?" keeping the first part
                        String split = state.substring(0, state.indexOf("?"));
                        bool mqttState = (split == "true") ? true : false;
                        cfg.config.MQTTEnabled = mqttState ? true : false;
                    }
                    log_i("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                }
                request->send(200, MIMETYPE_JSON, (cfg.config.MQTTEnabled == true) ?  "MQTT Disabled" : "MQTT Enabled"); });

    server.on("/data.json", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                  cfg.config.data_json = true;
                  my_delay(1L);
                  String temp = cfg.config.data_json_string;
                  request->send(200, MIMETYPE_JSON, temp);
                  temp = ""; });

    server.on("/api/reset/config", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                  cfg.resetConfig();
                  request->send(200, MIMETYPE_JSON, "Config has been Reset"); });

    server.on("/api/reset/device", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                  request->send(200, MIMETYPE_JSON, "Resetting Device");
                  my_delay(1L);
                  ESP.restart(); });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.onNotFound(notFound);
    server.begin();
}

// ######################## server functions #########################

/******************************************************************************
 * Function: Check HMSnetwork Connection Loop
 * Description: This function checks the HMSnetwork connection and reconnects if necessary - is called in the loop() function every 5 seconds
 * Parameters: None
 * Return: None
 ******************************************************************************/
void HMSnetwork::CheckNetworkLoop()
{
    // run current function every 5 seconds
    if (WiFi.status() != WL_CONNECTED)
    {
        wifiConnected = false;
        log_i("Wifi is not connected\n");
    }
    else
    {
        wifiConnected = true;
        log_i("Wifi is connected\n");
        log_i("[INFO]: WiFi Connected! Open http://%s in your browser\n", WiFi.localIP().toString().c_str());
    }
}

// BACKUP LEGACY FUNCTION
/******************************************************************************
 * Function: Connect to Wifi with fail to AP mode if no connection
 * Description: This connection will attempt to create a WiFi connection with the given SSID and password. If the connection fails, it will attempt to connect to a default Wifi AP.
 * Parameters: None
 * Return: None
 ******************************************************************************/
/* bool HMSnetwork::connectToApWithFailToStation()
{
    cfg.CreateDefaultConfig();
    if (!cfg.loadConfig())
    {
        log_i("[INFO]: Failed to load config");
    }
    else
    {
        log_i("[INFO]: Loaded config");
        // Load values saved in SPIFFS
        String SSID = cfg.config.WIFISSID;
        String PASS = cfg.config.WIFIPASS;
        String ntptime = cfg.config.NTPTIME;
        String ntptimeoffset = cfg.config.NTPTIMEOFFSET;
        String mdns = cfg.config.MDNS;
        String dhcpcheck = cfg.config.DHCPCHECK;

        if (!PRODUCTION)
        {
            // print it on the serial monitor
            log_i(PASS);
        }

        log_i(mdns);
        log_i(dhcpcheck);
    }

    WiFi.persistent(true);
    log_i("[INFO]: Configuring access point...");
    log_i("SSID:");
    log_i(cfg.config.WIFISSID);
    log_i("PASS:");
    log_i(cfg.config.WIFIPASS);

    WiFi.mode(WIFI_STA);
    if (cfg.config.WIFISSID[0] == '\0')
    {
        WiFi.reconnect();
    }
    else
    {
        WiFi.begin(cfg.config.WIFISSID, cfg.config.WIFIPASS);
    }

    int numberOfAttempts = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        numberOfAttempts++;
        delay(200);
        log_i(".");
        if (numberOfAttempts > 100)
        {
            WiFi.mode(WIFI_AP);
            // You can remove the password parameter if you want the AP to be open.
            log_i("Wifi Connect Failed. \r\nStarting AP. \r\nAP IP address: ");
            WiFi.softAP(WIFI_SSID, WIFI_PASS, 10, 1, 2);
            log_i(WiFi.softAPIP());
            return false;
            break;
        }
    }
    log_i("[INFO]: Connected! IP address: " + String(WiFi.localIP()));
    return true;
} */

void HMSnetwork::SetupServer()
{
    log_i("\n");
    log_i("System Information:\n");
    log_i("\n");
    log_i("PLatformI0 compile time: %s (%s)\n", __DATE__, __TIME__);
    log_i("PLatformI0 Unix compile time: %d\n", COMPILE_UNIX_TIME);
    log_i("Project directory: %s\n", PROJECT_PATH);
    log_i("Version: %d\n", VERSION);
    log_i("Heap: %d\n", ESP.getFreeHeap());
    log_i("SDK: %s\n", ESP.getSdkVersion());
    log_i("MAC address: %s\n", WiFi.macAddress().c_str());
    log_i("CPU Speed: %dMHz\n", ESP.getCpuFreqMHz());
    log_i("Flash Size: %dKB\n", ESP.getFlashChipSize());
    log_i("[INFO]: System Information Sent\n");
    log_i("\n");

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

void HMSnetwork::SetupWifiScan()
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // Disconnect from the access point if connected before
    my_delay(0.1L);

    log_i("[INFO]: Setup done\n");
}

bool HMSnetwork::LoopWifiScan()
{
    log_i("[INFO]: Beginning WIFI Network\n");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    log_i("[INFO]: scan done\n");
    if (n == 0)
    {
        log_i("[INFO]: no networks found\n");
        return false;
    }
    else
    {
        log_i("[INFO]: %d networks found\n", n);
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            log_i("%d: %s (%d) %s", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            my_delay(0.1L);
            return true;
        }
    }
    log_i("\n");

    // Wait a bit before scanning again
    my_delay(5L);
    return true;
}

HMSnetwork network;
