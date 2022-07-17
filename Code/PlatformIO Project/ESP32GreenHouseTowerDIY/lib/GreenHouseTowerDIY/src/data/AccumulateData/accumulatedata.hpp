#ifndef ACCUMULATEDATA_HPP
#define ACCUMULATEDATA_HPP
#include <defines.hpp>
#include <lists.h>
#include <vector>

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