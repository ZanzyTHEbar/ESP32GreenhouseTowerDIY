#pragma once
#ifndef TIMEDTASKS_HPP
#define TIMEDTASKS_HPP
#include <timeObj.h>

#include "local/data/AccumulateData/accumulatedata.hpp"

class TimedTasks : public timeObj
{
public:
  TimedTasks(AccumulateData *accumulateData);
  virtual ~TimedTasks(void);
  
  void accumulateSensorData(void);

private:
  // Timers
  timeObj _Timer_5s;
  AccumulateData *accumulateData;
};

#endif
