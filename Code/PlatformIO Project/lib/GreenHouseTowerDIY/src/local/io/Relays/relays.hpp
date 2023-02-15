#ifndef RELAYS_HPP
#define RELAYS_HPP
#include <Adafruit_MCP23008.h>
#include <Arduino.h>
#include <Wire.h>
#include <local/data/BackgroundTasks/taskManager.hpp>
#include <local/data/config/config.hpp>

class I2C_RelayBoard : public IObserver<ObserverEvent::CustomEvents>,
                       public timeObj {
 public:
  I2C_RelayBoard(GreenHouseConfig* config);
  virtual ~I2C_RelayBoard();

  void begin();
  void setRelay(uint8_t port, bool state);
  bool getRelay(uint8_t port);
  void update(ObserverEvent::CustomEvents event);
  void handleRelayTimer();

 private:
  Adafruit_MCP23008 relay;
  GreenHouseConfig* deviceConfig;
  timeObj* _timer;
};
#endif
