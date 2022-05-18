#include "bh1750.hpp"

hp_BH1750 BH1750_sensor; // create the sensor object

BH1750::BH1750()
{
}

BH1750::~BH1750()
{
}

void BH1750::setupLightSensor()
{
    // Initialize the sensor
    // use BH1750_TO_GROUND or BH1750_TO_VCC depending how you wired the address pin of the sensor.
#if bh1750_GND
    bool avail = BH1750_sensor.begin(BH1750_TO_GROUND); // will be false no sensor found
#endif // bh1750_GND
#if bh1750_VCC
    bool avail = BH1750_sensor.begin(BH1750_TO_VCC); // will be false no sensor found
#endif // bh1750_VCC                                                
}

void BH1750::loopLightSensor()
{
    // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
    my_delay(1L);
    log_i("Lux: %.3f", lux, DEC);
}

float BH1750::getLux()
{
}