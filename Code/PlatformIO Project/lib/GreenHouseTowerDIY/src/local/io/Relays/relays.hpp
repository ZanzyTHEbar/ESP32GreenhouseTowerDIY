#ifndef RELAYS_HPP
#define RELAYS_HPP
#include <Adafruit_MCP23008.h>
#include <Arduino.h>
#include <Wire.h>
#include <local/data/BackgroundTasks/taskManager.hpp>
#include <local/data/config/config.hpp>

// TODO: Add support for ATTiny85 based I2C relays

// TODO: Add support for TCA9548A I2C multiplexer

class I2C_RelayBoard : public IObserver<ObserverEvent::CustomEvents>,
                       public timeObj {
  Adafruit_MCP23008 relay;
  GreenHouseConfig* deviceConfig;
  timeObj* _timer;

 public:
  I2C_RelayBoard(GreenHouseConfig* config);
  virtual ~I2C_RelayBoard();

  void begin();
  void setRelay(uint8_t port, bool state);
  bool getRelay(uint8_t port);
  void handleRelayTimer();
  void eventListener(std::string result, uint8_t port);
  void update(ObserverEvent::CustomEvents event);
};
#endif
