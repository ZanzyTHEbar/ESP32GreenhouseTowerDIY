#ifndef MDNS_DISCOVERY_HPP
#define MDNS_DISCOVERY_HPP
#include <Arduino.h>
#include <ESPmDNS.h>
#include <data/statemanager/StateManager.hpp>
#include <utilities/helpers.hpp>

/**
 * @brief Namespace for mDNS Discovery
 */
namespace mDNSDiscovery {
  //******************************************************************************
  // * Function: Discover mDNS Broker
  // * Description: Find the mDNS broker and and port and write them to the
  // config struct
  // * Parameters: None
  // * Return: int - 1 if success, 0 if fail
  //******************************************************************************
  int discovermDNSBroker();
  void setMQTTBroker(const std::string& broker, int port);
  struct MQTTBroker {
    std::string broker;
    int port;
  };
  MQTTBroker brokerConfig;
  const MQTTBroker* getMQTTBroker();
}  // namespace mDNSDiscovery

#endif  // MDNS_DISCOVERY_HPP
