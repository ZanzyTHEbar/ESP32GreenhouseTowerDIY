#ifndef CALIBRATIONBUTTON_HPP
#define CALIBRATIONBUTTON_HPP
#include <blinker.h>    // The base LC_baseTools blinker class.
#include <mechButton.h> // The base LC_baseTools mechButton class.

#define DEF_HOLD_MS 10000     // How many ms we'll call a "long" button press.
#define __DEF_HOLD_MS__ 5000 // How many ms we'll call a "long" button press.

class CalibrationButton : public mechButton,
                          public timeObj
{
public:
    CalibrationButton(int inPin);
    virtual ~CalibrationButton(void);

    void setLongCallback(void (*funct)(void));
    void setCalibrationCallback(void (*funct)(void));
    virtual void idle(void);

private:
    timeObj _calibrationTimer; // The timer for the long button press.
    void setCalibrationTimer(void);
    void (*_longCallback)(void);
    void (*_calibrationCallback)(void);
};
extern CalibrationButton calibrationButton;
#endif // CALIBRATIONBUTTON_HPP