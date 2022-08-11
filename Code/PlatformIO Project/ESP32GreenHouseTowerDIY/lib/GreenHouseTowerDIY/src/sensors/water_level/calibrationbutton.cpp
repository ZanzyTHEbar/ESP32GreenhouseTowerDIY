#include "calibrationbutton.hpp"

CalibrationButton::CalibrationButton(uint8_t inPin) : mechButton(inPin),
                                                      timeObj(DEF_HOLD_MS, false),
                                                      _longCallback(nullptr),
                                                      _calibrationCallback(nullptr) {}

CalibrationButton::~CalibrationButton(void) {}

void CalibrationButton::setCalibrationTimer(void)
{
    _calibrationTimer.setTime(__DEF_HOLD_MS__);
}

void CalibrationButton::setLongCallback(funct_callback_t funct)
{
    _longCallback = funct;
    hookup();
}

void CalibrationButton::setCalibrationCallback(funct_callback_t funct)
{
    setCalibrationTimer();
    _calibrationCallback = funct;
    hookup();
}

void CalibrationButton::idle(void)
{
    this->trueFalse();
}

bool CalibrationButton::trueFalse(void)
{
    mechButton::trueFalse();
    if (!beenInitialized)
    {                                  // If not ready to run..
        pinMode(pinNum, INPUT_PULLUP); // Set up our pin.
        setAs = digitalRead(pinNum);   // The first thing we see is our initial state.
        beenInitialized = true;        // Note we are open for business.
        mTimer.start();                // And don't bug us again for this much time.
        _calibrationTimer.start();     // We start our timer.
        start();
    }
    else
    { // Else, we are already up and running..
        if (mTimer.ding())
        { // If the timer has expired..
            if (setAs == digitalRead(pinNum))
            {                 // If we get another reading like the saved state..
                checkNum = 0; // Reset to zero conflicting readings.
            }
            else
            {               // Else, its a conflicting reading..
                checkNum++; // Bump up checkNum.
                if (checkNum >= NUM_CHECKS)
                {                   // If we have enough conflicting readings..
                    setAs = !setAs; // Flip our state.
                    checkNum = 0;   // Reset to zero conflicting readings.

                    if (_longCallback)
                    {                    // We run out of time AND have a callback..
                        _longCallback(); // Call the callback.
                        reset();         // Turn the timer off.
                    }

                    if (_calibrationCallback)
                    {                              // We run out of time AND have a callback..
                        _calibrationCallback();    // Call the callback.
                        _calibrationTimer.reset(); // Turn the timer off.
                    }
                    else
                    {                 // Else, no callback..
                        takeAction(); // Do whatever action is required.
                    }
                }
            }
            mTimer.start();            // Don't bug us, restarting the timer..
            _calibrationTimer.start(); // We start our timer.
            start();
        }
    }
    return setAs; // In all cases, return how we are set.
}

//***************************************************
//*                                                 *
//*              END OF CalibrationButton           *
//*                                                 *
//***************************************************
