#ifndef MDNSMANAGER_HPP
#define MDNSMANAGER_HPP
#include <defines.hpp>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <observer.hpp>

#include "data/StateManager/StateManager.hpp"
#include "data/Config/config.hpp"

namespace mDNSManager
{
    class MDNSHandler : public IObserver
    {
    private:
        StateManager<ProgramStates::DeviceStates::MDNSState_e> *stateManager;
        Config *deviceConfig;

    public:
        MDNSHandler(StateManager<ProgramStates::DeviceStates::MDNSState_e> *stateManager, Config *deviceConfig);
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