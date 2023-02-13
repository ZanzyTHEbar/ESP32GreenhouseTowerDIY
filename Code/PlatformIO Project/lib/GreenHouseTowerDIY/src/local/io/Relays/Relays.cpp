#include "Relays.hpp"

Relays::Relays()
#if USE_PID
    : _Setpoint(),
      _Input(),
      _Output(),
      _WindowSize(5000),
      _windowStartTime(),
      myPID(&_Input, &_Output, &_Setpoint, 2, 5, 1, DIRECT)
#endif  // USE_PID
{
  // use a c++ ranged for loop to iterate through the relay pins
  for (int pin : cfg.config.relays_pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
#if USE_PID
  _windowStartTime = millis();
  // initialize the variables we're linked to
  _Setpoint = 80;
  // tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, _WindowSize);
  // turn the PID on
  myPID.SetMode(AUTOMATIC);
#endif  // USE_PID
}

Relays::~Relays() {}

/******************************************************************************
 * Function: Control the Relays
 * Description: This function turns the relays in the stack on and off.
 * Parameters: int, bool, long (in seconds)
 * Return: None
 ******************************************************************************/
void Relays::RelayOnOff(int relay, bool on, long double delay) {
  if (on) {
    digitalWrite(relay, HIGH);
  } else {
    digitalWrite(relay, LOW);
  }
  my_delay(delay);
}

/******************************************************************************
 * Function: Humidity Related Relay Control
 * Description: Initialise a PID controller to control a relay based on humidity
 *sensor readings Parameters: None Return: None Below PID Relay code is an
 *example of how to use the PID controller
 ******************************************************************************/
#if USE_PID
#if USE_SHT31_SENSOR
void Relays::HumRelayOnOff() {
  float climate_data = humidity.StackHumidity();
  Input = climate_data;
  myPID.Compute();

  // turn the output pin on/off based on pid output
  unsigned long now = millis();
  if (now - _windowStartTime > _WindowSize) {  // time to shift the Relay Window
    _windowStartTime += _WindowSize;
  }
  if (Output > now - _windowStartTime)
    digitalWrite(cfg.config.relays_pin[0], HIGH);
  else
    digitalWrite(cfg.config.relays_pin[0], LOW);
}
#endif  // USE_SHT31_SENSOR
#endif  // USE_PID

Relays Relay;
