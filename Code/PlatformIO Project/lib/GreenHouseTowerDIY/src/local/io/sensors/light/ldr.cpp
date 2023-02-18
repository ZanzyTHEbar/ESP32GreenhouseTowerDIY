#include "ldr.hpp"

LDR::LDR() : _GAMMA(0.7), _RL10(50)
{
  pinMode(LDR_PIN, INPUT);
}

LDR::~LDR() {}

float LDR::getLux()
{
  // Convert the analog value into lux value:
  float readLDR = analogRead(LDR_PIN);
  float voltage = readLDR / 4096.0 * 3.3;
  float resistance = 2000.0 * voltage / (1.0 - voltage / 3.3);
  float lux = pow(_RL10 * 1e3 * pow(10.0, _GAMMA) / resistance, (1.0 / _GAMMA));
  char buffer[100];
  dtostrf(lux, 10, 3, buffer);
  log_i("%s\n", buffer);
  Network_Utilities::my_delay(0.1L);
  return lux;
}
