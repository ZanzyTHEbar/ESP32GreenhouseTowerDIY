#include "LEDManager.hpp"

LEDManager::LEDManager(byte pin) : _ledPin(pin), _previousMillis(0), _ledState(false), _leds{0} {}

LEDManager::~LEDManager() {}

void LEDManager::begin()
{
    pinMode(_ledPin, OUTPUT);
    onOff(false);

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


void Switch(int state){
    // create switch case
    switch (state)
    {
    case 0:
        // do nothing
        break;
    case 1:
        // do nothing
        break;
    case 2:
        // do nothing
        break;
    case 3:
        // do nothing
        break;
    default:
        // do nothing
        break;
    }
}
LEDManager ledManager;