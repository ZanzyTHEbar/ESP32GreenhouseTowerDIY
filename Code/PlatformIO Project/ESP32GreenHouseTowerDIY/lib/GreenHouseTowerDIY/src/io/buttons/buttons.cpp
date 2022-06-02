#include "buttons.hpp"

// ***********************************************************
//                Buttons class code
// ***********************************************************

// Constructor, not much going on. Just passing the pin number in.
Buttons::Buttons(int inPin, bool input) : mechButton(inPin)
{
    if (!input)
    {
        pinMode(inPin, OUTPUT);
    }

    pinMode(inPin, INPUT);
    begin(); // Fire up the button. (Calls hookup() for idling.)
}

// Destructor, nothing allocated so nothing to do.
Buttons::~Buttons(void)
{
}

// begin, Needed somewhere to place the hookup() call. begin is traditional.
void Buttons::begin(void) { hookup(); }

// takeAction, this is what's called when there is no callback set.
void Buttons::takeAction(void)
{
    if (!setAs)
    {
    }
}

// Your standard sketch loop()
void Buttons::ButtonLoop(void)
{
    bool buttonState;
    idle();                              // Let all the idlers have time to do their thing.
    buttonState = trueFalse();           // Have a look at what the current button state is.
    digitalWrite(LED_PIN, !buttonState); // Since the button grounds when pushed, invert logic with !
}

Buttons buttons(int inPin);