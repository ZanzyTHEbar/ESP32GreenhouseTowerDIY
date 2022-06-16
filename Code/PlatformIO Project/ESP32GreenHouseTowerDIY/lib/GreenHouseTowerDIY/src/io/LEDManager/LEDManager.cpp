#include "LEDManager.hpp"

LEDManager::LEDManager(uint8_t pin) : _ledPin(pin) {}

LEDManager::~LEDManager() {}

void LEDManager::setupLED() const
{
    pinMode(_ledPin, OUTPUT);
}

void LEDManager::on() const
{
    digitalWrite(_ledPin, LOW);
}

void LEDManager::off() const
{
    digitalWrite(_ledPin, HIGH);
}

void LEDManager::blink(unsigned long time)
{
    on();
    my_delay(time);
    off();
}

void LEDManager::displayStatus()
{
}

LEDManager ledManager(LED_BUILTIN);