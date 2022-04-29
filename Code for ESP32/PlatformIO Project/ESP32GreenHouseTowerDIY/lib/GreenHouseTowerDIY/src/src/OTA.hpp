#ifndef OTA_HPP
#define OTA_HPP
#include <Arduino.h>
#include "defines.hpp"
#include <SecureOTA.hpp>

class OTA
{
public:
    OTA();
    virtual ~OTA();
    void setupOTA();
    void loopOTA();
    
    /* void checkFirmwareUpdates();
    void handleOTA();
    void handleOTA_error(int error);
    void handleOTA_progress(unsigned int progress, unsigned int total);
    void handleOTA_end();
    void handleOTA_start();
    void handleOTA_write(unsigned int size);
    void handleOTA_chunk(unsigned int index, uint8_t *data, unsigned int size); */

    // variables
private:
};

extern OTA ota;
#endif