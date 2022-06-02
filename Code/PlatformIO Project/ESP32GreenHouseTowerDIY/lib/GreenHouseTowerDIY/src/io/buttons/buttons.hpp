#ifndef BUTTONS_HPP
#define BUTTONS_HPP
#include <defines.hpp>
#include <TimeLib.h> // For timekeeping
#include <mechButton.h>
#include <idlers.h>

#define BUTTON_CALLBACK(name) void (*name)()
#define BUTTON_MESSAGE_CALLBACK(name) void (*name)(const char *topic, const uint8_t *payload, uint16_t length)

// ***********************************************************
//                Buttons class definition
//
//    Inherit from mechButton and create a class that does
//                  our custom bidding.
// ***********************************************************
class Buttons : public mechButton
{

public:
    Buttons(int inPin, bool input = false);
    virtual ~Buttons(void);

    void begin(void);
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

private:
    BUTTON_MESSAGE_CALLBACK(_messageCallback);
    BUTTON_CALLBACK(_connectedCallback);
    BUTTON_CALLBACK(_connectionFailedCallback);
};

extern Buttons buttons(int inPin);
#endif // BUTTONS_HPP