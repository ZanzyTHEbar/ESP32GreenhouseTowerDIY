/*
 bh1750.hpp - ESP32GreenHouseDIY  bh1750 Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef BH1750_HPP
#define BH1750_HPP
#include <defines.hpp>
#include <hp_BH1750.h>

class BH1750
{
public:
    // Constructor
    BH1750();
    virtual ~BH1750();

    // Initialize the library
    void setupLightSensor();
    void loopLightSensor();
    float getLux();

private:
    // Private variables
};

extern BH1750 bh1750;
#endif