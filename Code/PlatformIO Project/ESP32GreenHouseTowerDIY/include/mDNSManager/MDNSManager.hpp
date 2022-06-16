#ifndef MDNSMANAGER_HPP
#define MDNSMANAGER_HPP
#include <ESPmDNS.h>
#include <StateManager.hpp>
#include <storage.hpp>

namespace mDNSManager
{
    class MDNSHandler : public IObserver
    {
    private:
        StateManager *stateManager;
        Configuration *trackerConfig;

    public:
        MDNSHandler(StateManager *stateManager, Configuration *trackerConfig) : stateManager(stateManager), trackerConfig(trackerConfig) {}
        void startMDNS();
        void update(ObserverEvent::Event event);

        //******************************************************************************
        // * Function: Discover mDNS Broker
        // * Description: Find the mDNS broker and and port and write them to the config struct
        // * Parameters: None
        // * Return: int - 1 if success, 0 if fail
        //******************************************************************************
        int DiscovermDNSBroker();
    };
}

#endif // MDNSMANAGER_HPP