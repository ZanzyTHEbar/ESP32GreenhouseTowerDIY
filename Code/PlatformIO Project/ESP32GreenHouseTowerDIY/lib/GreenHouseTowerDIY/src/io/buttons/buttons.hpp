#ifndef BUTTONS_HPP
#define BUTTONS_HPP
#include <defines.hpp>
#include <TimeLib.h> // For timekeeping
#include <mechButton.h>
#include <idlers.h>

#define BUTTON_PIN 2 // Pin we'll hook the button to. The other side hooks to ground.
#define LED_PIN 13   // Usual pin number for built in LED.

// ***********************************************************
//                Buttons class definition
//
//    Inherit from mechButton and create a class that does
//                  our custom bidding.
// ***********************************************************
class Buttons : public mechButton
{

public:
    Buttons(int inPin);
    virtual ~Buttons(void);

    void begin(void);
    virtual void takeAction(void);
    void blank(int numBlanks);
    void line(int numLines);

    void ButtonSetup(void);
    void ButtonLoop(void);
};

extern Buttons buttons(int inPin);
#endif // BUTTONS_HPP