#include "WifiHandler.hpp"

void WiFiHandler::setupWifi(StateManager *stateManager, Configuration *trackerConfig)
{
    log_i("Initializing connection to wifi");

    std::vector<WiFiConfig> *networks = trackerConfig->getWifiConfigs();
    int connectionTimeout = 3000;

    for (auto networkIterator = networks->begin(); networkIterator != networks->end(); ++networkIterator)
    {
        log_i("Trying to connect using %s\n\r", networkIterator->ssid);

        int timeSpentConnecting = 0;
        WiFi.begin(networkIterator->ssid, networkIterator->password);

        while (timeSpentConnecting < connectionTimeout && !WiFi.isConnected())
        {
            Serial.print(".");
            timeSpentConnecting += 300;
            delay(300);
        }

        if (!WiFi.isConnected())
            log_e("\n\rCould not connect to %s, trying another network\n\r", networkIterator->ssid);
        else
        {
            log_i("\n\rSuccessfully connected to %s\n\r", networkIterator->ssid);
            stateManager->setState(_State::ConnectingToWifiSuccess);
            log_i("ESP will be streaming under 'http://%s:%s/80\r\n", WiFi.localIP().toString().c_str());
            log_i("ESP will be accepting commands under 'http://%s:%s/80/control\r\n", WiFi.localIP().toString().c_str());
            return;
        }
    }

    log_e("Could not connected to any of the specified networks, check the configuration and try again");
    stateManager->setState(_State::ConnectingToWifiSuccess);
}