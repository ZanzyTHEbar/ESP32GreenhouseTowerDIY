#include "waterlevelsensor.hpp"

//***********************************************************************************************************************
// * Capacitive Water Level Sensor Class
// * This class is used to read the water level from a capacitive sensor.
// * The sensor is connected to the ESP32 via a GPIO Touch pin.
//! * Manual calibration is needed!!!
//************************************************************************************************************************

TODO("Add manual calibration");

UltraSonicDistanceSensor _distanceSensor(ECHO_PIN, TRIG_PIN); // Initialize sensor that uses digital pins 13 and 12.

WaterLevelSensor::WaterLevelSensor() {}

WaterLevelSensor::~WaterLevelSensor() {}

int WaterLevelSensor::readWaterLevelCapacitive()
{
    const byte touchPin = CAP_WATER_LEVEL_SENSOR_PIN;
    int touchValue = touchRead(touchPin);
    int percentage = map(touchValue, 0, 4095, 0, 100);
    log_d("Water Level Sensor Raw Readings: %d", touchValue);
    log_i("Water Level Sensor Percentage: %d", percentage);
    return percentage;
}

void WaterLevelSensor::begin()
{
    _radius = RES_RADIUS_1;
    _height = RES_HEIGHT;
}

double WaterLevelSensor::readSensor()
{
    double distance = _distanceSensor.measureDistanceCm(tower_temp.temp_sensor_results.temp[0]);
    log_i("°C - Distance: %.3f cm", distance, DEC);
    // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
    my_delay(1L);
    return distance;
}

int WaterLevelSensor::readWaterLevelUltraSonic()
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
    log_d("True Water Level Distance: %.3f cm", readSensor(), DEC);

    if (isnan(p))
    {
        log_e("Error: %s", "Sensor Value is NaN");
        return 0.0;
    }

    // pause between readings, in seconds:
    my_delay(30L);
    return (int)p;
}

int WaterLevelSensor::getWaterLevel()
{
    #if USE_CAP
    return readWaterLevelCapacitive();
    #else if USE_UC
    return readWaterLevelUltraSonic();
    #endif // USE_CAP
}

WaterLevelSensor waterlevelSensor;