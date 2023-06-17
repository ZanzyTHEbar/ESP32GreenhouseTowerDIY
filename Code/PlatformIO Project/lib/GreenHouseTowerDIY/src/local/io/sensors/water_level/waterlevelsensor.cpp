#include "waterlevelsensor.hpp"

//***********************************************************************************************************************
// * Capacitive Water Level Sensor Class
// * This class is used to read the water level from a capacitive sensor.
// * The sensor is connected to the ESP32 via a GPIO Touch pin.
//! * Manual calibration is needed!!!
//************************************************************************************************************************

// TODO: Add set method for radius  and height
// TODO: Migrate this to a pressure sensor

WaterLevelSensor::WaterLevelSensor(TowerTemp& _towerTemp)
    : _radius(),
      _height(),
      _towerTemp(_towerTemp),
      _distanceSensor(TRIG_PIN, ECHO_PIN) {}
WaterLevelSensor::~WaterLevelSensor() {}

double WaterLevelSensor::readSensor() {
  Network_Utilities::my_delay(1L);
  double distance =
      _distanceSensor.measureDistanceCm(_towerTemp.temp_sensor_results[0]);
  log_d("[WaterLevelSensor]: Distance: %.3f cm", distance, DEC);
  log_d("[WaterLevelSensor]: Temperature: %.3f °C",
        _towerTemp.temp_sensor_results[0], DEC);
  // Every 1 second, do a measurement using the sensor and print the distance
  // in centimeters.
  return distance;
}

float WaterLevelSensor::read() {
  if (readSensor() <= 0.0) {
    log_i("[WaterLevelSensor]: Distance greater than 400cm");
    log_i("[WaterLevelSensor]: Failed to read ultrasonic sensor.");
    return 0.0;
  }

  //* tank diameter and height, in centimeters
  double diameter = pow(_radius, 2.0);
  double v = volume();
  double stock = ((diameter * PI) * (_height - readSensor()) / 1000.0);
  log_i("[WaterLevelSensor]: Stock is: %.3f liters", stock, DEC);
  double p = (stock / v) * 100.0;

  log_i("[WaterLevelSensor]: Percent Full: %.3f", p, DEC);
  log_d("[WaterLevelSensor]: True Water Level Distance: %.3f cm", readSensor(),
        DEC);

  if (isnan(p)) {
    log_e("[WaterLevelSensor]: Error: %s", "Sensor Value is NaN");
    return 0.0;
  }
  return stock;
}

double WaterLevelSensor::volume() {
  double diameter = pow(_radius, 2.0);
  double v = ((diameter * PI) * (_height) / 1000.0);
  return v;
}

const std::string& WaterLevelSensor::getSensorName() {
  static const std::string name = "water_level_sensor";
  return name;
}

void WaterLevelSensor::accept(Visitor<SensorInterface<float>>& visitor) {
  visitor.visit(this);
}

//***********************************************************************************************************************

WaterLevelPercentage::WaterLevelPercentage(WaterLevelSensor& waterLevelSensor)
    : _waterLevelSensor(waterLevelSensor) {}

float WaterLevelPercentage::read() {
  float stock = _waterLevelSensor.read();
  float percentage = (stock / _waterLevelSensor.volume()) * 100.0;

  if (isnan(percentage)) {
    log_e("[WaterLevelSensor]: Error: %s", "Sensor Value is NaN");
    return 0;
  }

  log_i("[WaterLevelSensor]: Percent Full: %.3f", percentage, DEC);
  return percentage;
}

const std::string& WaterLevelPercentage::getSensorName() {
  static const std::string name = "water_level_percentage";
  return name;
}

void WaterLevelPercentage::accept(Visitor<SensorInterface<float>>& visitor) {
  visitor.visit(this);
}
