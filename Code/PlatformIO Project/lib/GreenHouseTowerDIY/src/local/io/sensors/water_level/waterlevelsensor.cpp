#include "waterlevelsensor.hpp"

//***********************************************************************************************************************
// * Capacitive Water Level Sensor Class
// * This class is used to read the water level from a capacitive sensor.
// * The sensor is connected to the ESP32 via a GPIO Touch pin.
//! * Manual calibration is needed!!!
//************************************************************************************************************************

WaterLevelSensor::WaterLevelSensor(TowerTemp& _towerTemp)
    : _towerTemp(_towerTemp), _distanceSensor(TRIG_PIN, ECHO_PIN) {}
WaterLevelSensor::~WaterLevelSensor() {}

void WaterLevelSensor::begin() {
    _radius = RES_RADIUS_1;
    _height = RES_HEIGHT;
}

double WaterLevelSensor::readSensor() {
    Network_Utilities::my_delay(1L);
    double distance = _distanceSensor.measureDistanceCm(
        _towerTemp.temp_sensor_results.temp[0]);
    log_d("Distance: %.3f cm", distance, DEC);
    log_d("Temperature: %.3f °C", _towerTemp.temp_sensor_results.temp[0], DEC);
    // Every 1 second, do a measurement using the sensor and print the distance
    // in centimeters.
    return distance;
}

WaterLevelSensor::Data_t WaterLevelSensor::readWaterLevelUltraSonic() {
    if (readSensor() <= 0.0) {
        log_i("Distance greater than 400cm");
        log_i("Failed to read ultrasonic sensor.");
        return {0, 0};
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

    if (isnan(p)) {
        log_e("Error: %s", "Sensor Value is NaN");
        return {0, 0};
    }
    result = {stock, p};
    return result;
}
