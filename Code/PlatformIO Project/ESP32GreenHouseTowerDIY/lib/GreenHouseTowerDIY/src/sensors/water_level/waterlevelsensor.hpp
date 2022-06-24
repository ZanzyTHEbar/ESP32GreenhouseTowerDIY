/*
 WaterlevelSensor.h - ESP32GreenHouseDIY Distance Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef WATERLEVELSENSOR_HPP
#define WATERLEVELSENSOR_HPP
#include <defines.hpp>
#include <HCSR04.h>

class WaterLevelSensor
{
public:
    // Constructor
    WaterLevelSensor();
    virtual ~WaterLevelSensor();
    void begin();
    double readSensor();
    // Read the water level
    int readWaterLevelUltraSonic();
    int readWaterLevelCapacitive();

private:
    double _radius;
    double _height;
    // Private variables
};

extern WaterLevelSensor waterlevelSensor;
#endif