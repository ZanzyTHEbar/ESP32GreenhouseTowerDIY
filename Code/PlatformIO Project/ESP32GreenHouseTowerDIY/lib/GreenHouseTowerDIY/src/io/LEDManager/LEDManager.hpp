#ifndef LEDMANAGER_HPP
#define LEDMANAGER_HPP
#include <defines.hpp>

class LEDManager
{
public:
    explicit LEDManager(uint8_t pin);
    virtual ~LEDManager();

    void setupLED() const;
    void onOff(bool state) const;
    void blink(unsigned long time);
    void displayStatus();

private:
    uint8_t _ledPin;
};

extern LEDManager ledManager;

#endif // LEDMANAGER_HPP