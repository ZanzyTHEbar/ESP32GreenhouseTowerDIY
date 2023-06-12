#ifndef MDNS_DISCOVERY_HPP
#define MDNS_DISCOVERY_HPP
#include <Arduino.h>
#include <ESPmDNS.h>
#include <data/statemanager/StateManager.hpp>
#include <utilities/helpers.hpp>
#include "local/data/config/config.hpp"

/**
 * @brief Namespace for mDNS Discovery
 */
class mDNSDiscovery {
  GreenHouseConfig& _deviceConfig;

 public:
  mDNSDiscovery(GreenHouseConfig& config);
  virtual ~mDNSDiscovery();
  //******************************************************************************
  // * Function: Discover mDNS Broker
  // * Description: Find the mDNS broker and and port and write them to the
  // config struct
  // * Parameters: None
  // * Return: int - 1 if success, 0 if fail
  //******************************************************************************
  bool discovermDNSBroker();
};  // namespace mDNSDiscovery

#endif  // MDNS_DISCOVERY_HPP
