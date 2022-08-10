/*
 TowerTemp.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef TOWERTEMP_HPP
#define TOWERTEMP_HPP
// Data wire is plugged into port 42 on the ESP32
#define ONE_WIRE_BUS 42
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <vector>
#include <memory>

class TowerTemp
{
public:
    TowerTemp();
    virtual ~TowerTemp();
    void SetupSensors();
    void checkSensors();
    void setSensorCount();
    int getSensorCount();

    struct Temp
    {
        std::vector<float> temp;
    };

    Temp temp_sensor_results;

    Temp getTempC();
    Temp getTempF();

private:
    void readAddresses(DeviceAddress deviceAddress);
    void printAddress(DeviceAddress deviceAddress);

private:
    int _sensors_count;
    // Setup a oneWire instance to communicate with any OneWire devices
    std::shared_ptr<OneWire> oneWire;
    // Pass our oneWire reference to Dallas Temperature.
    std::shared_ptr<DallasTemperature> sensors;
    // variable to hold device addresses
    DeviceAddress temp_sensor_addresses;
};
extern TowerTemp tower_temp;
#endif