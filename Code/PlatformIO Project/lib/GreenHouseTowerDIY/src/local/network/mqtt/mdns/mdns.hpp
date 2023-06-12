#ifndef MDNS_DISCOVERY_HPP
#define MDNS_DISCOVERY_HPP
#include "local/data/config/config.hpp"

/**
 * @brief Namespace for mDNS Discovery
 */
namespace mDNSDiscovery {
  bool discovermDNSBroker(GreenHouseConfig& _deviceConfig);
}  // namespace mDNSDiscovery
#endif  // MDNS_DISCOVERY_HPP
