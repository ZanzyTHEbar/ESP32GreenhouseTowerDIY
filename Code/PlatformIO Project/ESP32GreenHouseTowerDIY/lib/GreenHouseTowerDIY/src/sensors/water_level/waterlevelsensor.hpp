/*
 WaterlevelSensor.h - ESP32GreenHouseDIY Distance Sensor library
 Copyright (c) 2021 ZanzyTHEbar
 */
#pragma once
#ifndef WATERLEVELSENSOR_HPP
#define WATERLEVELSENSOR_HPP
#include <defines.hpp>
#include <HCSR04.h>
#if USE_CAP
#include "calibrationbutton.hpp"
#endif // USE_CAP

class WaterLevelSensor
{
public:
    // Constructor
    WaterLevelSensor();
    virtual ~WaterLevelSensor();
    void begin();
    double readSensor();
    // Read the water level

    int getWaterLevel();
    int getPercentage();

private:
    // Private variables
    double _radius;
    double _height;
    bool _activateCalibration;
    byte _depth;
    byte _depthRange;
    int _depthArray[100];
    int _qNumberReadings[100];
    int _qNumberDepth[100];
    struct Calibration_t
    {
        int _min;
        int _max;
    };
    Calibration_t _calibration;

    // Private functions
    int readWaterLevelUltraSonic();
    int readWaterLevelCapacitive();
    void calibrateSensor();
    void setCapSensorRange();
    int convertToQNumber(int readings);
    void convertToQNumber(int *readings, int *depth);
    // Private Friends
    friend void holdCallback(void);
    friend void quickCallback(void);
    friend void longholdCallback(void);
};

extern WaterLevelSensor waterlevelSensor;
#endif