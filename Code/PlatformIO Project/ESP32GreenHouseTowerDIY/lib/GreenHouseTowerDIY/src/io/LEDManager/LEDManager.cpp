#include "LEDManager.hpp"

LEDManager::LEDManager(uint8_t pin) : _ledPin(pin) {}

LEDManager::~LEDManager() {}

void LEDManager::setupLED() const
{
    pinMode(_ledPin, OUTPUT);
}

void LEDManager::onOff(bool state) const
{
    digitalWrite(_ledPin, state);
}

void LEDManager::blink(unsigned long time)
{
    onOff(true);
    my_delay(time);
    onOff(false);
}

void LEDManager::displayStatus()
{
}

LEDManager ledManager(LED_BUILTIN);