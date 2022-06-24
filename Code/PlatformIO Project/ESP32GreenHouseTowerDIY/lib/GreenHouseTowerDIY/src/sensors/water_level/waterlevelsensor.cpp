#include "waterlevelsensor.hpp"

//***********************************************************************************************************************
// * Capacitive Water Level Sensor Class
// * This class is used to read the water level from a capacitive sensor.
// * The sensor is connected to the ESP32 via a GPIO Touch pin.
//! * Manual calibration is needed!!!
//************************************************************************************************************************

TODO("Add manual calibration");

UltraSonicDistanceSensor _distanceSensor(ECHO_PIN, TRIG_PIN); // Initialize sensor that uses digital pins 13 and 12.

// This is the function that is called on a quick click.
void quickCallback(void)
{
    if (waterlevelSensor._activateCalibration)
    {
        if (calibrationButton.trueFalse())
        {
            // the button is not being pressed..
            return;
        }

        // the button is being pressed..
        waterlevelSensor.calibrateSensor();
    }
}

// This is the function that is called on a long click.
void holdCallback(void)
{
    // Long press has happened.
    waterlevelSensor._activateCalibration = !waterlevelSensor._activateCalibration;
}

WaterLevelSensor::WaterLevelSensor() : _activateCalibration(false), _depth(0) {}

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
#if USE_CAP
    calibrationButton.setCallback(quickCallback);    // When the button gets clicked. Call this function.
    calibrationButton.setLongCallback(holdCallback); // If it's been held down for a "long" time call this.
#endif                                               // USE_CAP
}

//***********************************************************************************************************************
//*
//* 1. Place ruler or tape measure in the water next to the sensor.
//* 2a. Plug the sensor into the PCB and the PCB into the computer.
//* 2b. Press and hold the sensor calibrate button on the PCB for 5 seconds.
//* 3. Press the calibrate sensor button (do not hold), wait for the sensor to read a value, and then lower the sensor by 1cm.
//* 4. Repeat step #3 for each depth (this is to mitigate error) until you get the min and max values of the sensor, the sensor being fully submerged.
//* 5. Press and hold the sensor calibrate button for 5 seconds to exit calibration.
//************************************************************************************************************************
void WaterLevelSensor::calibrateSensor()
{
    byte numtoaverage = 5;

    int _readings[] = {0};
    for (byte i = 0; i < numtoaverage; i++)
    {
        _readings[i] += getWaterLevel();
        my_delay(0.1L);
    }

    for (byte i = 0; i < numtoaverage; i++)
    {
        _readings[i] = _readings[i] / numtoaverage;
    }
    _calibration = {,};
    _depth++;
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