#include "buttons.hpp"

// ***********************************************************
//                Buttons class code
// ***********************************************************

// Constructor, not much going on. Just passing the pin number in.
Buttons::Buttons(int inPin) : mechButton(inPin)
{
}

// Destructor, nothing allocated so nothing to do.
Buttons::~Buttons(void)
{
}

// begin, Needed somewhere to place the hookup() call. begin is traditional.
void Buttons::begin(void) { hookup(); }

// blank, print some blanks.
void Buttons::blank(int numBlanks)
{
    for (int i = 0; i < numBlanks; i++)
    {
        Serial.print(' ');
    }
}

// line, give us some blank lines
void Buttons::line(int numLines)
{

    for (int i = 0; i < numLines; i++)
    {
        Serial.println();
    }
}

// takeAction, this is what's called when there is no callback set.
void Buttons::takeAction(void)
{

    if (setAs)
    {
        blank(36);
        Serial.println(F("Live long and prosper."));
    }
    else
    {
        line(4);
        blank(10);
        Serial.println(F("                                     ---------------------------==="));
        blank(10);
        Serial.println(F("            __                      ( |                          =="));
        blank(10);
        Serial.println(F("         /------\\                    ---------------------------="));
        blank(10);
        Serial.println(F("------------------------------           |  |"));
        blank(10);
        Serial.println(F("\\____________________________/]          |  |"));
        blank(10);
        Serial.println(F("         --------       \\     \\          |  |"));
        blank(10);
        Serial.println(F("                         \\     \\         |  |"));
        blank(10);
        Serial.println(F("                          \\ --------_____|  |__"));
        blank(10);
        Serial.println(F("                         | |              --  /"));
        blank(10);
        Serial.println(F("                        -||                  ||"));
        blank(10);
        Serial.println(F("                         | |__________/------== "));
        line(2);
    }
}

void Buttons::ButtonSetup(void)
{
    pinMode(LED_PIN, OUTPUT); // Set up the LED pin for output.
    begin();                  // Fire up the button. (Calls hookup() for idling.)
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