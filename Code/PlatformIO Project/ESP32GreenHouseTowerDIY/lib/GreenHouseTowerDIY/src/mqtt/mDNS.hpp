#include <defines.hpp>
#include <ESPmDNS.h>

/**
 * @brief Namespace for mDNS Discovery
 */
namespace mDNSDiscovery
{
    //******************************************************************************
    // * Function: Discover mDNS Broker
    // * Description: Find the mDNS broker and and port and write them to the config struct
    // * Parameters: None
    // * Return: int - 1 if success, 0 if fail
    //******************************************************************************
    int DiscovermDNSBroker();
}