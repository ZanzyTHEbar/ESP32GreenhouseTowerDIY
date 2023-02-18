#ifndef MDNS_DISCOVERY_HPP
#define MDNS_DISCOVERY_HPP
#include <Arduino.h>
#include <ESPmDNS.h>
#include <data/statemanager/StateManager.hpp>
#include <local/data/BackgroundTasks/taskManager.hpp>
#include <utilities/helpers.hpp>

/**
 * @brief Namespace for mDNS Discovery
 */
class mDNSDiscovery
{
  TaskManager *taskManager;

public:
  mDNSDiscovery(TaskManager *taskManager);
  virtual ~mDNSDiscovery();
  //******************************************************************************
  // * Function: Discover mDNS Broker
  // * Description: Find the mDNS broker and and port and write them to the
  // config struct
  // * Parameters: None
  // * Return: int - 1 if success, 0 if fail
  //******************************************************************************
  bool discovermDNSBroker();
}; // namespace mDNSDiscovery

#endif // MDNS_DISCOVERY_HPP
