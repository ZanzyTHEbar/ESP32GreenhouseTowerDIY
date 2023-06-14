#include "mDNS.hpp"
#include <Arduino.h>
#include <ESPmDNS.h>

//******************************************************************************
// * Function: Discover mDNS Broker
// * Description: Find the mDNS broker and and port return them
// * Parameters: None
// * Return: int - 1 if success, 0 if fail
//******************************************************************************
bool mDNSDiscovery::discovermDNSBroker(ProjectConfig& networkConfig,
                                       GreenHouseConfig& deviceConfig) {
  IPAddress mqttServer;
  // check if there is a WiFi connection
  if (wifiStateManager.getCurrentState() != WiFiState_e::WiFiState_Connected) {
    log_i("[mDNS Broker Discovery]: connected!\n");
    log_i("[mDNS Broker Discovery]: Setting up mDNS: ");
    if (!MDNS.begin(networkConfig.getMDNSConfig().hostname.c_str())) {
      log_e("[Fail]: Error setting up mDNS lookup\n");
      return false;
    } else {
      log_i("[OK]\n");
      log_i("[mDNS Broker Discovery]: Querying MQTT broker: ");
      int n = MDNS.queryService("mqtt", "tcp") ||
              MDNS.queryService("_mqtt", "_tcp");
      if (n == 0) {
        // No service found
        log_e("[Fail]: No mqtt service found on your network\n");
        return false;
      } else {
        int mqttPort;
        // Found one or more MQTT services - use the first one.
        log_i("[OK]\n");
        mqttServer = MDNS.IP(0);
        mqttPort = MDNS.port(0);
        deviceConfig.setMQTTBroker(mqttServer.toString().c_str(), mqttPort);
        log_i("[mDNS Broker Discovery]: MQTT broker found at: %s\n: %d",
              mqttServer.toString().c_str(), mqttPort);
        return true;
      }
    }
  }
  return false;
}
