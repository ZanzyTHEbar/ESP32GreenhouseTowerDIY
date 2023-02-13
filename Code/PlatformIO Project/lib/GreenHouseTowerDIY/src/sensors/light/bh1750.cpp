#include "bh1750.hpp"

hp_BH1750 BH1750_sensor; // create the sensor object

BH1750::BH1750() {}

BH1750::~BH1750() {}

void BH1750::begin()
{
    // Initialize the sensor
    // use BH1750_TO_GROUND or BH1750_TO_VCC depending how you wired the address pin of the sensor.
#if bh1750_GND
    bool avail = BH1750_sensor.begin(BH1750_TO_GROUND); // will be false no sensor found
#endif                                                  // bh1750_GND
#if bh1750_VCC
    bool avail = BH1750_sensor.begin(BH1750_TO_VCC); // will be false no sensor found
#endif                                               // bh1750_VCC
    if (!avail)
    {
        Serial.println("No BH1750 sensor found");
    }
    else
    {
        Serial.println("BH1750 sensor found");
#if bh1750_FAST
        BH1750_sensor.calibrateTiming();
#endif                         // bh1750_FAST
        BH1750_sensor.start(); // start the first measurement in setup
    }
}

float BH1750::getLux()
{
#if bh1750_FAST
    if (BH1750_sensor.hasValue() == true)
#else
    if (BH1750_sensor.hasValue())
#endif // bh1750_FAST
    {  // non blocking reading
        float lux = BH1750_sensor.getLux();
        log_i("Light lux level: %.3f", lux);
        BH1750_sensor.start();
        return lux;
    }
    return 0;
}

BH1750 bh1750; // create the sensor object