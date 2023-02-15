#include "relays.hpp"

I2C_RelayBoard::I2C_RelayBoard(GreenHouseConfig* config)
    : relay(), deviceConfig(config) {}

I2C_RelayBoard::~I2C_RelayBoard() {}

void I2C_RelayBoard::begin() {
  relay.begin(0x20);  // use default address
  for (auto device : deviceConfig->config.relays) {
    relay.pinMode(device.port, OUTPUT);
  }
}

void I2C_RelayBoard::setRelay(uint8_t port, bool state) {
  relay.digitalWrite(port, state);
}

bool I2C_RelayBoard::getRelay(uint8_t port) {
  return relay.digitalRead(port);
}

void I2C_RelayBoard::update(ObserverEvent::CustomEvents event) {
  switch (event) {
    case ObserverEvent::CustomEvents::relaysConfigChanged:
      this->begin();
      break;
    default:
      break;
  }
}
