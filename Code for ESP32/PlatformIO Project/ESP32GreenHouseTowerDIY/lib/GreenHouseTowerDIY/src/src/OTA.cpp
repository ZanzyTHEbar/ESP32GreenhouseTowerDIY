#include "OTA.hpp"

const uint16_t OTA_CHECK_INTERVAL = 3000; // ms
uint32_t _lastOTACheck = 0;

OTA::OTA()
{
}

OTA::~OTA()
{
}

void OTA::setupOTA()
{
    _lastOTACheck = millis();
}

void OTA::loopOTA()
{
    if ((millis() - OTA_CHECK_INTERVAL) > _lastOTACheck)
    {
        _lastOTACheck = millis();
        checkFirmwareUpdates();
    }
}

OTA ota;