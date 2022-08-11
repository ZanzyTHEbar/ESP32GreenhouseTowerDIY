#ifndef CALIBRATIONBUTTON_HPP
#define CALIBRATIONBUTTON_HPP
#include <blinker.h>    // The base LC_baseTools blinker class.
#include <mechButton.h> // The base LC_baseTools mechButton class.

#define DEF_HOLD_MS 10000     // How many ms we'll call a "long" button press.
#define __DEF_HOLD_MS__ 5000 // How many ms we'll call a "long" button press.
#define NUM_CHECKS 4         // Number of conflicting checks in a row that tells us to change.
class CalibrationButton : public mechButton,
                          public timeObj
{
public:
    CalibrationButton(uint8_t inPin);
    virtual ~CalibrationButton(void);

    void setLongCallback(funct_callback_t funct);
    void setCalibrationCallback(funct_callback_t funct);
    virtual void idle(void);
    virtual bool trueFalse(void);

private:
    timeObj _calibrationTimer; // The timer for the long button press.
    void setCalibrationTimer(void);
    funct_callback_t _longCallback;
    funct_callback_t _calibrationCallback;
};
#endif // CALIBRATIONBUTTON_HPP