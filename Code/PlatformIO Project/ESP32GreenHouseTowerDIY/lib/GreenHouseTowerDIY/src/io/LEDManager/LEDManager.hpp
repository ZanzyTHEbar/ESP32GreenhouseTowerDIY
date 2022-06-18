#ifndef LEDMANAGER_HPP
#define LEDMANAGER_HPP
#include <defines.hpp>

class LEDManager
{
public:
    LEDManager(byte pin);
    virtual ~LEDManager();

    void setupLED() const;
    void onOff(bool state) const;
    void blink(unsigned long time);
    void displayStatus();

private:
    byte _ledPin;
    unsigned long _previousMillis;
    bool _ledState;
};

extern LEDManager ledManager;

#endif // LEDMANAGER_HPP