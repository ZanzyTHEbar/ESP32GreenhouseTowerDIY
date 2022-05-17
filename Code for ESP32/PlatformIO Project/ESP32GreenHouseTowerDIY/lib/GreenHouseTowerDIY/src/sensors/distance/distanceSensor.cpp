#include "distanceSensor.hpp"
#include <bits/stdc++.h>

UltraSonicDistanceSensor _distanceSensor(ECHO, TRIGGER); // Initialize sensor that uses digital pins 13 and 12.

// Constructor
DistanceSensor::DistanceSensor()
{
    distance = _distanceSensor.measureDistanceCm(temp_sensor_results.temp[0]);
    // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
    my_delay(1L);
}

// Destructor
DistanceSensor::~DistanceSensor()
{
}

void DistanceSensor::setupDistanceSensor()
{
    log_i("°C - Distance: %.3f cm", distance, DEC);
}

double DistanceSensor::readWaterLevel()
{
    // tank diameter and height, in centimeters
    double radius = pow(100.0, 2.0);
    double height = 200.0;

    // pause between readings, in seconds:
    int readings = 30;
    double pi = 2.0 * acos(0.0);
    try
    {
        double v = ((radius * pi) * (height) / 1000.0);
        double stock = ((radius * pi) * (height - distance) / 1000.0);
        log_i("Stock is: %.3f liters", stock, DEC);
        double p = (stock / v) * 100.0;

        log_i("Percent Full: %.3f", p, DEC);
        log_i("True Distance: %.3f cm", distance, DEC);

        if ((int)p <= 25)
        {
        }
        else if ((int)p > 25 && (int)p < 50)
        {
        }
        else if ((int)p > 75)
        {
        }
        else
        {
        }

        my_delay(readings);

        if (*((char *)typeid(p).name()) == 'f' || *(typeid(p).name()) == 'i')
        {
            return p;
        }
        else
        {
            log_i("Invalid sensor readings.");
        }
    }
    catch (const std::exception &e)
    {
        log_i("%c \n", e.what());
        log_i("Failed to read ultrasonic sensor.");
    }
}

DistanceSensor distanceSensor;