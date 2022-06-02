#ifndef BUTTONS_HPP
#define BUTTONS_HPP
#include <defines.hpp>
#include <mechButton.h>
#include <idlers.h>

#define Buttons_DEBUG 0
#define BUTTON_CALLBACK(name) void (*name)()
#define BUTTON_MESSAGE_CALLBACK(name) void (*name)()

// ***********************************************************
//                Buttons class definition
//
//    Inherit from mechButton and create a class that does
//                  our custom bidding.
// ***********************************************************
class Buttons : public mechButton
{

public:
    inline static Buttons *instance()
    {
        return _instance;
    }

    Buttons(byte inPin, bool input = false);
    virtual ~Buttons(void);

    bool begin(void);
    void processState(void);
    virtual void takeAction(void);
    void ButtonLoop(void);

    /**
     * Given callback will be called for each received message from the button.
     *
     * @param callback
     */
    inline void onMessage(BUTTON_MESSAGE_CALLBACK(callback))
    {
        _messageCallback = callback;
    }

    /**
     * Given callback will be called each time the connection with a button is acquired.
     *
     * @param callback
     */
    inline void onConnected(BUTTON_CALLBACK(callback))
    {
        _connectedCallback = callback;
    }

    /**
     * Given callback will be called each time the library fails to connect to a button.
     *
     * @param callback
     */
    inline void onConnectionFailed(BUTTON_CALLBACK(callback))
    {
        _connectionFailedCallback = callback;
    }

    inline bool getPin(byte &_pin)
    {
        if (_pin > 0)
        {
            _inPin = _pin;
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    static Buttons *_instance;
    BUTTON_MESSAGE_CALLBACK(_messageCallback);
    BUTTON_CALLBACK(_connectedCallback);
    BUTTON_CALLBACK(_connectionFailedCallback);
    byte _inPin;
    bool _input;
    bool _initialized;
};

extern Buttons buttons(int inPin);
#endif // BUTTONS_HPP