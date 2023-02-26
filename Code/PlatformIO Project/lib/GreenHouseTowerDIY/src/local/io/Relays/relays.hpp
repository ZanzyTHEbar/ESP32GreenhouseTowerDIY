#ifndef RELAYS_HPP
#define RELAYS_HPP
#include <Adafruit_MCP23008.h>
#include <Arduino.h>
#include <Wire.h>
#include <local/data/BackgroundTasks/taskManager.hpp>
#include <local/data/config/config.hpp>

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

  /**
   * @brief Event listener for the Relys
   * @note This function is used to listen for events from the MQTT
   * broker.
   * @param topic The topic of the event
   * @param payload The payload of the event
   * @param length The length of the payload
   * @return void
   */
  void eventListener(const std::string& result);
};
#endif
