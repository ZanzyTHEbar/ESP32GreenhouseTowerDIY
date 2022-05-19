#include "distanceSensor.hpp"

UltraSonicDistanceSensor _distanceSensor(ECHO, TRIGGER); // Initialize sensor that uses digital pins 13 and 12.

// Constructor
DistanceSensor::DistanceSensor()
{
}

// Destructor
DistanceSensor::~DistanceSensor()
{
}

double DistanceSensor::readSensor()
{
    double distance = _distanceSensor.measureDistanceCm(Tower_Temp.temp_sensor_results.temp[0]);
    log_i("°C - Distance: %.3f cm", distance, DEC);
    // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
    my_delay(1L);
    return distance;
}

double DistanceSensor::readWaterLevel()
{
    try
    {
        // tank diameter and height, in centimeters
        double radius = pow(100.0, 2.0);
        double height = 200.0;
        double v = ((radius * PI) * (height) / 1000.0);
        double stock = ((radius * PI) * (height - readSensor()) / 1000.0);
        log_i("Stock is: %.3f liters", stock, DEC);
        double p = (stock / v) * 100.0;

        log_i("Percent Full: %.3f", p, DEC);
        log_i("True Distance: %.3f cm", readSensor(), DEC);

        if (*((char *)typeid(p).name()) == 'f' || *(typeid(p).name()) == 'i')
        {
            return p;
        }
    }
    catch (const std::exception &e)
    {
        log_i("%c \n", e.what());
        log_i("Failed to read ultrasonic sensor.");
    }

    log_i("Invalid sensor readings.");
    return 0.0;
}

void DistanceSensor::indicateWaterLevel()
{
    // pause between readings, in seconds:
    long readings = 30L;
    double p = readWaterLevel();
    try
    {
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
    }
    catch (const std::exception &e)
    {
        log_i("%c \n", e.what());
        log_i("Failed to read ultrasonic sensor.");
    }
}

DistanceSensor distanceSensor;