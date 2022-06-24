#include "distanceSensor.hpp"

UltraSonicDistanceSensor _distanceSensor(ECHO_PIN, TRIG_PIN); // Initialize sensor that uses digital pins 13 and 12.

// Constructor
DistanceSensor::DistanceSensor() {}

// Destructor
DistanceSensor::~DistanceSensor() {}

void DistanceSensor::begin()
{
    _radius = RES_RADIUS_1;
    _height = RES_HEIGHT;
    _leds[0] = RED_LED_PIN;
    _leds[1] = GREEN_LED_PIN;
    _leds[2] = BLUE_LED_PIN;

    for (auto &led : _leds)
    {
        if (led > 0)
        {
            pinMode(led, OUTPUT);
        }
    }
}

double DistanceSensor::readSensor()
{
    double distance = _distanceSensor.measureDistanceCm(tower_temp.temp_sensor_results.temp[0]);
    log_i("°C - Distance: %.3f cm", distance, DEC);
    // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
    my_delay(1L);
    return distance;
}

int DistanceSensor::readWaterLevel()
{
    if (readSensor() <= 0.0)
    {
        log_i("Failed to read ultrasonic sensor.");
        return 0.0;
    }

    // tank diameter and height, in centimeters
    double diameter = pow(_radius, 2.0);
    double height = _height;
    double v = ((diameter * PI) * (height) / 1000.0);
    double stock = ((diameter * PI) * (height - readSensor()) / 1000.0);
    log_i("Stock is: %.3f liters", stock, DEC);
    double p = (stock / v) * 100.0;

    log_i("Percent Full: %.3f", p, DEC);
    log_i("True Distance: %.3f cm", readSensor(), DEC);

    if (isnan(p))
    {
        log_e("Error: %s", "Sensor Value is NaN");
        return 0.0;
    }

    // pause between readings, in seconds:
    my_delay(30L);
    return (int)p;
}

void DistanceSensor::indicateWaterLevel()
{
    int p = readWaterLevel();
    if (p <= 25)
    {
        digitalWrite(_leds[1], LOW);
        digitalWrite(_leds[2], LOW);
        digitalWrite(_leds[0], HIGH);
    }
    else if (p > 25 && p < 50)
    {
        digitalWrite(_leds[0], LOW);
        digitalWrite(_leds[2], LOW);
        digitalWrite(_leds[1], HIGH);
    }
    else if (p > 75)
    {
        digitalWrite(_leds[0], LOW);
        digitalWrite(_leds[1], LOW);
        digitalWrite(_leds[2], HIGH);
    }
    else
    {
        for (auto &led : _leds)
        {
            digitalWrite(led, LOW);
        }
    }
}

DistanceSensor distanceSensor;