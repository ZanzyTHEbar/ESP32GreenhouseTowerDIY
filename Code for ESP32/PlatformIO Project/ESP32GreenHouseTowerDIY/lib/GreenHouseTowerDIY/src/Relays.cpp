#include "Relays.hpp"

double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT); // Specify the links and initial tuning parameters
int WindowSize = 5000;
unsigned long windowStartTime;

Relays::Relays()
{
}

Relays::~Relays()
{
}

/******************************************************************************
 * Function: Controll the Relays
 * Description: This function turns the relays in the stack on and off.
 * Parameters: int, bool, long (in seconds)
 * Return: None
 ******************************************************************************/
void Relays::RelayOnOff(int relay, bool on, long double delay)
{
    if (on)
    {
        digitalWrite(relay, HIGH);
    }
    else
    {
        digitalWrite(relay, LOW);
    }
    my_delay(delay);
}

/******************************************************************************
 * Function: Setup PID Controller
 * Description: This function sets up the PID controller
 * Parameters: None
 * Return: None
 ******************************************************************************/
void Relays::SetupPID()
{
    windowStartTime = millis();

    // initialize the variables we're linked to
    Setpoint = 80;

    // tell the PID to range between 0 and the full window size
    myPID.SetOutputLimits(0, WindowSize);

    // turn the PID on
    myPID.SetMode(AUTOMATIC);
}

/******************************************************************************
 * Function: Humidity Related Relay Control
 * Description: Initialise a PID controller to control a relay based on humidity sensor readings
 * Parameters: None
 * Return: None
 * SFM3003 Mass Air _Flow Sensor code to be integrated
 * Below PID Relay code is an example of how to use the PID controller
 * This code should only be used during the Charging phase. Integrate State Machine to use this code
 ******************************************************************************/
void Relays::HumRelayOnOff()
{
    float climate_data = humidity.StackHumidity();
    Input = climate_data;
    myPID.Compute();

    // turn the output pin on/off based on pid output
    unsigned long now = millis();
    if (now - windowStartTime > WindowSize)
    { // time to shift the Relay Window
        windowStartTime += WindowSize;
    }
    if (Output > now - windowStartTime)
        digitalWrite(cfg.config.relays_pin[0], HIGH);
    else
        digitalWrite(cfg.config.relays_pin[0], LOW);
}

Relays Relay;