/*
 celltemp.h - HBAT library
 Copyright (c) 2021 ZanzyTHEbar
 */

#ifndef CELLTEMP_HPP
#define CELLTEMP_HPP
// Data wire is plugged into port 42 on the ESP32
#define ONE_WIRE_BUS 42
#include "defines.hpp"
#include <OneWire.h>
#include <DallasTemperature.h>

struct Temp
{
    float temp[10];
};

extern Temp cell_temp_sensor_results;
class CellTemp
{
public:
    CellTemp();
    virtual ~CellTemp();
    void SetupSensors();
    void readAddresses(DeviceAddress deviceAddress);
    void printAddress(DeviceAddress deviceAddress);
    Temp ReadTempSensorData();

private:
};

extern int sensors_count;
extern CellTemp Cell_Temp;
#endif