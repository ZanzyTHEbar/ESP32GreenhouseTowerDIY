#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP
#include <defines.hpp>
#include <ArduinoJson.h>
#include <lists.h>
#include <vector>

// data Struct
#include "data/Config/config.hpp"
#include "data/StateManager/StateManager.hpp"

// Light Sensors
#if USE_BH1750
#include "sensors/light/bh1750.hpp"
#else
#include "sensors/light/ldr.hpp"
#endif // USE_BH1750

// Temp Sensors
#include "sensors/temperature/towertemp.hpp"

// Humidity Sensors
#include "sensors/humidity/Humidity.hpp"

// Water Level Sensors
#include "sensors/water_level/waterlevelsensor.hpp"

// PH Sensors
#if ENABLE_PH_SUPPORT
#include "sensors/pH/pHsensor.hpp"
#endif // ENABLE_PH_SUPPORT

// Timed tasks
#include "data/BackgroundTasks/timedtasks.hpp"

class AccumulateData : public linkListObj
{
public:
    AccumulateData();
    virtual ~AccumulateData();

    void begin();
    virtual void execute(void);

    void InitAccumulateData();
    bool SendData();

    // Variables
    bool inList;

private:
    // Stack Data to send
    int _maxTemp;
    int _numTempSensors;
};

extern AccumulateData accumulatedata;

#endif