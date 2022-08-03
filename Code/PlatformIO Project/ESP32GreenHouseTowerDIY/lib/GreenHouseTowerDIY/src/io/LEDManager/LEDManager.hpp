#ifndef LEDMANAGER_HPP
#define LEDMANAGER_HPP
#include <Arduino.h>

class LEDManager
{
public:
    LEDManager(byte pin);
    virtual ~LEDManager();

    void begin();
    void onOff(bool state) const;
    void blink(unsigned long time);
    void displayStatus();
    void indicateWaterLevel(int waterlevel);

private:
    byte _ledPin;
    byte _leds[3];
    unsigned long _previousMillis;
    bool _ledState;
};
extern LEDManager ledManager;
#endif // LEDMANAGER_HPP