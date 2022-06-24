#ifndef CALIBRATIONBUTTON_HPP
#define CALIBRATIONBUTTON_HPP
#include <blinker.h>    // The base LC_baseTools blinker class.
#include <mechButton.h> // The base LC_baseTools mechButton class.

#define DEF_HOLD_MS 1000 // How many ms we'll call a "long" button press.

class CalibrationButton : public mechButton,
                          public timeObj
{
public:
    CalibrationButton(int inPin);
    virtual ~CalibrationButton(void);

    void setLongCallback(void (*funct)(void));
    virtual void idle(void);

    void (*longCallback)(void);
};
extern CalibrationButton calibrationButton;
#endif // CALIBRATIONBUTTON_HPP