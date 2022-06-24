#ifndef LEDMANAGER_HPP
#define LEDMANAGER_HPP
#include <defines.hpp>

class LEDManager
{
public:
    LEDManager(byte pin);
    virtual ~LEDManager();

    void setupLED();
    void onOff(bool state) const;
    void blink(unsigned long time);
    void displayStatus();
    void indicateWaterLevel(int waterLevel);

private:
    byte _ledPin;
    byte _leds[3];
    unsigned long _previousMillis;
    bool _ledState;
};

extern LEDManager ledManager;

#endif // LEDMANAGER_HPP