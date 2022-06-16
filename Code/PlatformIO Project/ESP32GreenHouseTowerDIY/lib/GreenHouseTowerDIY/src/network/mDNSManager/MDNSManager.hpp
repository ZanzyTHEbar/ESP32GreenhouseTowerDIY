#ifndef MDNSMANAGER_HPP
#define MDNSMANAGER_HPP
#include <defines.hpp>
#include <ESPmDNS.h>

namespace mDNSManager
{
    class MDNSHandler : public IObserver
    {
    private:
        StateManager *stateManager;
        Config *deviceConfig;

    public:
        MDNSHandler(StateManager *stateManager, Config *deviceConfig);
        virtual ~MDNSHandler();
        bool startMDNS();
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