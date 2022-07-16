#include "LEDManager.hpp"
template <typename T>
LEDManager<T>::LEDManager(T pin) : _ledPin(pin), _previousMillis(0), _ledState(false) {}

template <typename T>
LEDManager<T>::~LEDManager() {}

template <typename T>
void LEDManager<T>::begin()
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

template <typename T>
void LEDManager<T>::onOff(bool state) const
{
    digitalWrite(_ledPin, state);
}

template <typename T>
void LEDManager<T>::blink(unsigned long time)
{
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= time)
    {
        _previousMillis = currentMillis;
        _ledState = !_ledState;
        onOff(_ledState);
    }
}

template <typename T>
void LEDManager<T>::displayStatus()
{
}

template <typename T>
void LEDManager<T>::indicateWaterLevel(int waterlevel)
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