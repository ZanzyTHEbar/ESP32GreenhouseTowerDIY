#include "LEDManager.hpp"

LEDManager::LEDManager(uint8_t pin) : _ledPin(pin), _previousMillis(0), _ledState(false) {}

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
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= time)
    {
        _previousMillis = currentMillis;
        _ledState = !_ledState;
        onOff(_ledState);
    }
}

void LEDManager::displayStatus()
{
}

LEDManager ledManager(LED_BUILTIN); //! TODO: replace with a custom pin number