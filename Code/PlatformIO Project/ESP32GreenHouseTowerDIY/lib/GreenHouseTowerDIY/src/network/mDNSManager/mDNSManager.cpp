#include "mDNSManager.hpp"

namespace mDNSManager
{
    MDNSHandler::MDNSHandler(StateManager *stateManager, Config *deviceConfig) : stateManager(stateManager),
                                                                                 deviceConfig(deviceConfig) {}

    MDNSHandler::~MDNSHandler()
    {
    }

    bool MDNSHandler::startMDNS()
    {
        auto localConfig = deviceConfig->getDeviceConfig();
        if (!MDNS.begin(localConfig->hostname))
        {
            stateManager->setState(_State::MDNSError);
            log_e("Error initializing MDNS");
            return false;
        }

        stateManager->setState(_State::MDNSSuccess);
        MDNS.addService("GreenhouseTowerDIY", "tcp", 80);
        MDNS.addServiceTxt("GreenhouseTowerDIY", "tcp", "webserver_port", String(80));
        log_i("MDNS initialized!");
        return true;
    }

    void MDNSHandler::update(ObserverEvent::Event event)
    {
        if (event == ObserverEvent::deviceConfigUpdated)
        {
            MDNS.end();
            startMDNS();
        }
    }

    //******************************************************************************
    // * Function: Discover mDNS Broker
    // * Description: Find the mDNS broker and port - return them
    // * Parameters: None
    // * Return: int, 1 if success 0 if fail
    //******************************************************************************
    int MDNSHandler::DiscovermDNSBroker()
    {
        IPAddress mqttServer;
        // check if there is a WiFi connection
        if (WiFi.status() == WL_CONNECTED)
        {
            log_i("[mDNS Broker Discovery]: connected!\n");

            log_i("[mDNS Broker Discovery]: Setting up mDNS: ");
            if (!startMDNS())
            {
                log_e("[mDNS Broker Discovery]: failed to start mDNS!");
                return 0;
            }

            log_i("[OK]\n");
            log_i("[mDNS Broker Discovery]: Querying MQTT broker: ");

            int n = MDNS.queryService("mqtt", "tcp") || MDNS.queryService("_mqtt", "_tcp");

            if (n == 0)
            {
                // No service found
                log_e("[Fail] No mqtt service found on your network\n");
                return 0;
            }

            int mqttPort;
            // Found one or more MQTT services - use the first one.
            log_i("[OK]\n");
            mqttServer = MDNS.IP(0);
            mqttPort = MDNS.port(0);
            heapStr(&(cfg.config.MQTTBroker), mqttServer.toString().c_str());
            cfg.config.MQTTPort = mqttPort;

            switch (mqttPort)
            {
            case MQTT_PORT:
                log_i("[mDNS Broker Discovery]: MQTT port is insecure - running on port: %d\n", mqttPort);
                break;

            case MQTT_SECURE_PORT:
                log_i("[mDNS Broker Discovery]: MQTT port is secure - running on port: %d\n", mqttPort);
                break;

            case 0:
                log_i("[mDNS Broker Discovery]: MQTT port is not set - running on port: %d\n", mqttPort);
                break;

            default:
                log_i("[mDNS Broker Discovery]: MQTT port is on an unusual port - running on port: %d\n", mqttPort);
                break;
            }

            log_i("[mDNS Broker Discovery]: MQTT broker found at: %s\n: %d", cfg.config.MQTTBroker, cfg.config.MQTTPort);
            return 1;
        }
        return 0;
    }
}