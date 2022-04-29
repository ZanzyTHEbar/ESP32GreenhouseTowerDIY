#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP
#include "defines.hpp"

class AccumulateData
{
public:
    AccumulateData();
    virtual ~AccumulateData();

    void InitAccumulateData();
    int ChargeStatus();
    bool SendData();

private:
    // Stack Data to send
    int maxVoltage;
    int maxTemp;
};

extern AccumulateData accumulatedata;

#endif