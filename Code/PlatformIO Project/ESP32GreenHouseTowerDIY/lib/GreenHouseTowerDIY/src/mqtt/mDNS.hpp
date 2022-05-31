#include <defines.hpp>
#include <ESPmDNS.h>

// ############## functions to update current server settings ###################
int DiscovermDNSBroker()
{
    IPAddress mqttServer;
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

            int n = MDNS.queryService("mqtt", "tcp") || MDNS.queryService("_mqtt", "_tcp");

            if (n == 0)
            {
                // No service found
                log_i("[Fail]\n");
                return 0;
            }
            else
            {
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
        }
    }
    return 0;
}