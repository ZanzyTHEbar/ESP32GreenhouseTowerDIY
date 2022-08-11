#ifndef OTA_HPP
#define OTA_HPP
#include <defines.hpp>
#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include "data/Config/config.hpp"

class OTA
{
public:
    OTA(Config *_deviceConfig);
    virtual ~OTA();

    void SetupOTA();

    void HandleOTAUpdate();

private:
    unsigned long _bootTimestamp = 0;
    bool _isOtaEnabled = true;
    Config *_deviceConfig;
};
extern OTA ota;
#endif // OTA_HPP