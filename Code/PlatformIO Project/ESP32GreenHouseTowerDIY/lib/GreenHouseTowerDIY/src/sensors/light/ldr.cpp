#include "ldr.hpp"

const float GAMMA = 0.7;
const float RL10 = 50;

LDR::LDR()
{
}

LDR::~LDR()
{
}

void LDR::setupLDR()
{
    pinMode(LDR_PIN, INPUT);
}

float LDR::getLux()
{
    // Convert the analog value into lux value:
    float readLDR = analogRead(LDR_PIN);
    float voltage = readLDR / 4096.0 * 3.3;
    float resistance = 2000.0 * voltage / (1.0 - voltage / 3.3);
    float lux = pow(RL10 * 1e3 * pow(10.0, GAMMA) / resistance, (1.0 / GAMMA));
    char buffer[100];
    dtostrf(lux, 10, 3, buffer);
    log_i("%s\n", buffer);
    my_delay(0.1L);
    return lux;
}

void LDR::loopLDR()
{
    if (getLux() <= 7)
    {
        Relay.RelayOnOff(pump._pump_relay_pin, false, 0.1L);
    }
    else
    {
        Relay.RelayOnOff(pump._pump_relay_pin, true, 0.1L);
    }
}

LDR ldr;