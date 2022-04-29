#include "mdns.hpp"

// ######################## Multicast DNS #########################
IPAddress mqttServer;
String mdnsdotlocalurl = "hmshbat";

HMSmdns::HMSmdns()
{
    // constructor
}

HMSmdns::~HMSmdns()
{
    // destructor
}

int HMSmdns::DiscovermDNSBroker()
{
    // check if there is a WiFi connection
    if (WiFi.status() == WL_CONNECTED)
    {
        log_i("[mDNS Broker Discovery]: connected!\n");

        log_i("[mDNS Broker Discovery]: Setting up mDNS: ");
        if (!MDNS.begin(mqtt_mDNS_clientId))
        {
            log_i("[Fail]\n");
        }
        else
        {
            log_i("[OK]\n");
            log_i("[mDNS Broker Discovery]: Querying MQTT broker: ");

            int n = MDNS.queryService("mqtt", "tcp");

            if (n == 0)
            {
                // No service found
                log_i("[Fail]\n");
                return 0;
            }
            else
            {
                int mqttPort;
                // Found one or more MQTT service - use the first one.
                log_i("[OK]\n");
                mqttServer = MDNS.IP(0);
                mqttPort = MDNS.port(0);
                heapStr(&(cfg.config.MQTTBroker), mqttServer.toString().c_str());
                log_i("[mDNS Broker Discovery]: The port is:%d\n", mqttPort);
                cfg.config.MQTTPort = mqttPort;
                log_i("[mDNS Broker Discovery]: MQTT broker found at: %s\n", mqttServer.toString().c_str());
                log_i("%s", cfg.config.MQTTBroker);
                return 1;
            }
        }
        return 1;
    }
    return 0;
}

// ######################## Call this OR SetupWebServer - not both #########################
void HMSmdns::SetupmDNSServer()
{
    // ######################## Multicast DNS #########################
    log_i("Setting up mDNS: \n");
    // Set up mDNS responder:
    // - first argument is the domain name, in this example
    //   the fully-qualified domain name is "esp32.local"
    // - second argument is the IP address to advertise
    //   we send our IP address on the WiFi HMSmdns
    if (!MDNS.begin(mdnsdotlocalurl.c_str()))
    {
        log_i("[INFO]: Error setting up MDNS responder!\n");
        while (1)
        {
            delay(1000);
        }
    }
    log_i("[INFO]: mDNS responder started\n");

    // Start TCP (HTTP) server
    network.SetupWebServer();
    log_i("[INFO]: TCP server started\n");

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
}

bool HMSmdns::mDNSLoop()
{
    // ######################## Multicast DNS #########################
    // Check if a new espClient has connected
    if (WiFi.status() == WL_CONNECTED)
    {
        if (!espClient)
        {
            return false;
        }
        // Wait for data from espClient to become available
        while (!espClient.available())
        {
            delay(1);
        }

        // Read the first line of HTTP request
        String req = espClient.readStringUntil('\r');

        // First line of HTTP request looks like "GET /path HTTP/1.1"
        // Retrieve the "/path" part by finding the spaces
        int addr_start = req.indexOf(' ');
        int addr_end = req.indexOf(' ', addr_start + 1);
        if (addr_start == -1 || addr_end == -1)
        {
            log_i("Invalid request: %s\n", req.c_str());
            return false;
        }
        req = req.substring(addr_start + 1, addr_end);
        log_i("Request: %s\n", req.c_str());

        String s;
        if (req == "/")
        {
            IPAddress ip = WiFi.localIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP32 at ";
            s += ipStr;
            s += "</html>\r\n\r\n";
            log_i("[INFO]: Sending 200\n");
        }
        else
        {
            s = "HTTP/1.1 404 Not Found\r\n\r\n";
            log_i("[INFO]: Sending 404\n");
        }
        espClient.print(s);

        espClient.stop();
        log_i("[INFO]: Done with espClient\n");
    }
    return false;
}

HMSmdns Mdns;