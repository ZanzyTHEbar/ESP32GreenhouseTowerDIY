#pragma once
#ifndef TIMEDTASKS_HPP
#define TIMEDTASKS_HPP
#include "defines.hpp"
#include <timeObj.h>
/* #include <idlers.h> */

class TimedTasks : public timeObj
{
public:
  TimedTasks();
  virtual ~TimedTasks();

  void setupTimers();
  void updateCurrentData();
  void ScanI2CBus();
  void accumulateSensorData();
  void checkNetwork();

private:
  // Timers
  timeObj Timer_1s;
  timeObj Timer_5s;
  timeObj Timer_5s_2;
  timeObj Timer_10s;
  timeObj Timer_10s_2;
  timeObj Timer_30s;
  timeObj Timer_1m;
  timeObj Timer_5m;
};

extern TimedTasks timedTasks;

#endif