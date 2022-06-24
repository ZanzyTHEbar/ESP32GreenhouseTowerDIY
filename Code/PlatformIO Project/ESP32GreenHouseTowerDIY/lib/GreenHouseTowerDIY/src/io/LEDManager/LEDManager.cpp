#include "LEDManager.hpp"

LEDManager::LEDManager(uint8_t pin) : _ledPin(pin), _previousMillis(0), _ledState(false) {}

LEDManager::~LEDManager() {}

void LEDManager::setupLED()
{
    pinMode(_ledPin, OUTPUT);
    onOff(false);

    // Water Level Indication
    _leds[0] = RED_LED_PIN;
    _leds[1] = GREEN_LED_PIN;
    _leds[2] = BLUE_LED_PIN;

    for (auto &led : _leds)
    {
        if (led > 0)
        {
            pinMode(led, OUTPUT);
        }
    }
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

void LEDManager::indicateWaterLevel(int waterlevel)
{
    int p = waterlevel;
    if (p <= 25)
    {
        digitalWrite(_leds[1], LOW);
        digitalWrite(_leds[2], LOW);
        digitalWrite(_leds[0], HIGH);
    }
    else if (p > 25 && p < 50)
    {
        digitalWrite(_leds[0], LOW);
        digitalWrite(_leds[2], LOW);
        digitalWrite(_leds[1], HIGH);
    }
    else if (p > 75)
    {
        digitalWrite(_leds[0], LOW);
        digitalWrite(_leds[1], LOW);
        digitalWrite(_leds[2], HIGH);
    }
}

LEDManager ledManager(LED_BUILTIN); //! TODO: replace with a custom pin number