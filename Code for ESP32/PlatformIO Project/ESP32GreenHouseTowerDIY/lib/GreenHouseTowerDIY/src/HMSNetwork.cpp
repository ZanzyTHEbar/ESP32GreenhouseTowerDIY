#include "HMSnetwork.hpp"

WiFiClient espClient;

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

bool HMSnetwork::SetupNetworkStack()
{
    String SSID;
    String PASS;

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

        if (!PRODUCTION)
        {
            // print it on the serial monitor
            log_i("%s\n", PASS.c_str());
        }

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

            WiFi.mode(WIFI_STA);
            localIP.fromString(WiFi.localIP().toString());
            gateway.fromString(WiFi.gatewayIP().toString());
            subnet.fromString(WiFi.subnetMask().toString());

            if (!WiFi.config(localIP, gateway, subnet))
            {
                log_e("[INFO]: STA Failed to configure\n");
                return false;
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

        log_i("[INFO]: Configuring access point...\n");
        WiFi.mode(WIFI_AP);
        WiFi.setTxPower(WIFI_POWER_11dBm);

        // You can remove the password parameter if you want the AP to be open.
        log_i("Wifi Connection Failed. \r\nStarting AP. \r\nAP IP address: ");
        IPAddress IP = WiFi.softAPIP();
        log_i("[INFO]: AP IP address: %s.\r\n", IP.toString().c_str());

        if (!PRODUCTION)
        {
            WiFi.softAP("HBAT_HMS", NULL, 10, 1, 2); // AP mode without password
        }
        else
        {
            WiFi.softAP(WIFI_SSID, WIFI_PASS, 10, 0, 3); // AP mode with password
        }

        networkRoutes(); // call the network routes function to setup the routes
    }
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
