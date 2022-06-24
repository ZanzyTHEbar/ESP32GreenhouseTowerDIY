#include "calibrationbutton.hpp"

CalibrationButton::CalibrationButton(int inPin) : mechButton(inPin),
                                                  timeObj(DEF_HOLD_MS, false)
{
    longCallback = NULL;
}

CalibrationButton::~CalibrationButton(void) {}

void CalibrationButton::setLongCallback(void (*funct)(void))
{
    longCallback = funct;
    hookup();
}

void CalibrationButton::idle(void)
{
    bool state;

    state = setAs; // Save the state we have now.
    trueFalse();   // Do the mechButton checking state stuff.
    if (state != setAs)
    { // If there was a state change..
        if (!setAs)
        {            // If button was pressed..
            start(); // We start our timer.
        }
        else
        {            // Else, the button was let up.
            reset(); // Turn the timer off.
        }
    }
    else
    { // Else the button has not changed..
        if (!setAs)
        { // If the button is being held down..
            if (ding() && longCallback)
            {                   // We run out of time AND have a callback..
                longCallback(); // Call the callback.
                reset();        // Turn the timer off.
            }
        }
    }
}

CalibrationButton calibrationButton(CALIBRATION_BUTTON_PIN);

//***************************************************
//*                                                 *
//*              END OF CalibrationButton           *
//*                                                 *
//***************************************************
