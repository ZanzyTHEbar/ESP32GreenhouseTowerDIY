/*
 TowerTemp.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef TOWERTEMP_HPP
#define TOWERTEMP_HPP
// Data wire is plugged into port 42 on the ESP32
#define ONE_WIRE_BUS 42
#include <defines.hpp>
#include <OneWire.h>
#include <DallasTemperature.h>

class TowerTemp
{
public:
    TowerTemp();
    virtual ~TowerTemp();
    void SetupSensors();
    void readAddresses(DeviceAddress deviceAddress);
    void printAddress(DeviceAddress deviceAddress);
    float checkSensors();

    struct Temp
    {
        float temp[10];
    };

    Temp temp_sensor_results;

    Temp ReadTempSensorData();
    Temp GetTempF();

private:
};

extern int sensors_count;
extern TowerTemp tower_temp;
#endif