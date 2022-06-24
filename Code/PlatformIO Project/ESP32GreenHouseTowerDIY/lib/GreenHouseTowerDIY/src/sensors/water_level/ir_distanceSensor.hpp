/*
 ir_distanceSensor.h - ESP32GreenHouseDIY Distance Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef IR_DISTANCESENSOR_HPP
#define IR_DISTANCESENSOR_HPP
#include <defines.hpp>
#include <>

class IRDistanceSensor
{
public:
    // Constructor
    IRDistanceSensor();
    virtual ~IRDistanceSensor();

    // Initialize the library
    double readSensor();
    double readWaterLevel();
    void indicateWaterLevel();

private:
    // Private variables
};

extern IRDistanceSensor ir_distanceSensor;
#endif