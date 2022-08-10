#include "calibrationbutton.hpp"

CalibrationButton::CalibrationButton(int inPin) : mechButton(inPin),
                                                  timeObj(DEF_HOLD_MS, false),
                                                  _longCallback(nullptr),
                                                  _calibrationCallback(nullptr) {}

CalibrationButton::~CalibrationButton(void) {}

void CalibrationButton::setCalibrationTimer(void)
{
    _calibrationTimer.setTime(__DEF_HOLD_MS__);
}

void CalibrationButton::setLongCallback(void (*funct)(void))
{
    _longCallback = funct;
    hookup();
}

void CalibrationButton::setCalibrationCallback(void (*funct)(void))
{
    setCalibrationTimer();
    _calibrationCallback = funct;
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
        {                              // If button was pressed..
            start();                   // We start our timer.
            _calibrationTimer.start(); // We start our timer.
        }
        else
        {                              // Else, the button was let up.
            reset();                   // Turn the timer off.
            _calibrationTimer.reset(); // Turn the timer off.
        }
    }
    else
    { // Else the button has not changed..
        if (!setAs)
        { // If the button is being held down..
            if (ding() && _longCallback)
            {                    // We run out of time AND have a callback..
                _longCallback(); // Call the callback.
                reset();         // Turn the timer off.
            }

            if (_calibrationTimer.ding() && _calibrationCallback)
            {                              // We run out of time AND have a callback..
                _calibrationCallback();    // Call the callback.
                _calibrationTimer.reset(); // Turn the timer off.
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
