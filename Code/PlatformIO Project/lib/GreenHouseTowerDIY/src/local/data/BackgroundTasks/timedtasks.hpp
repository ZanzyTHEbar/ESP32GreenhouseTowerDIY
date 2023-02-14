#pragma once
#ifndef TIMEDTASKS_HPP
#define TIMEDTASKS_HPP
#include <timeObj.h>

#include "local/data/AccumulateData/accumulatedata.hpp"

class TimedTasks : public timeObj {
 private:
  typedef void (*callback_t)(void);

  // Timers
  timeObj _Timer_5s;
  AccumulateData* accumulateData;

 public:
  TimedTasks(AccumulateData* accumulateData);
  virtual ~TimedTasks(void);
  void accumulateSensorData(void);
  void dynamicTimer();

  struct Tasks_t {
    callback_t callback;
    int timeMS;
  };

  std::vector<Tasks_t> tasks;

  void setTask(Tasks_t task);
};

#endif
