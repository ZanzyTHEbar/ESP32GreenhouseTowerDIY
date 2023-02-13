
#include "timedtasks.hpp"

TimedTasks::TimedTasks(AccumulateData *accumulateData) : _Timer_5s(5000),
                                                         accumulateData(accumulateData) {}

TimedTasks::~TimedTasks(void) {}

void TimedTasks::accumulateSensorData(void)
{
  if (_Timer_5s.ding())
  {
    accumulateData->accumulateData();
    _Timer_5s.start();
  }
}
