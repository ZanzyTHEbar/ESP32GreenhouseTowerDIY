#ifndef LEDMANAGER_HPP
#define LEDMANAGER_HPP
#include <Arduino.h>

template <class T>
class LEDManager
{
public:
    LEDManager(T pin);
    virtual ~LEDManager();

    void begin();
    void onOff(bool state) const;
    void blink(unsigned long time);
    void displayStatus();
    void indicateWaterLevel(int waterlevel);

private:
    T _ledPin;
    unsigned long _previousMillis;
    bool _ledState;
};

#endif // LEDMANAGER_HPP