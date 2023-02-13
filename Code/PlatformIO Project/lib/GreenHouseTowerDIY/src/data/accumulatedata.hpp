#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP
#include <defines.hpp>

class AccumulateData
{
public:
    AccumulateData();
    virtual ~AccumulateData();

    void InitAccumulateData();
    int ChargeStatus();
    bool SendData();

    struct Data
    {
        // Variables
        float humidity;
        float humidity_temp;
        float humidity_sht31;
        float humidity_sht31_2;
        float humidity_temp_sht31;
        float humidity_temp_sht31_2;
        float humidity_sht31_average;
        float humidity_temp_sht31_average;
        float temp_sensors[10];
        int flow_rate;
        int flow_rate_sensor_temp;
    };
    Data config;

private:
    // Stack Data to send
    int _maxTemp;
    int _numTempSensors;
};

extern AccumulateData accumulatedata;

#endif