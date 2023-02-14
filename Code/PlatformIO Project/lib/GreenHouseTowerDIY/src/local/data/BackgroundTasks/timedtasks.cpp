
#include "timedtasks.hpp"

TimedTasks::TimedTasks(AccumulateData* accumulateData)
    : _Timer_5s(5000), accumulateData(accumulateData) {}

TimedTasks::~TimedTasks(void) {}

void TimedTasks::accumulateSensorData(void) {
  if (_Timer_5s.ding()) {
    accumulateData->accumulateData();
    _Timer_5s.start();
  }
}

// Create a function that takes in a time and generates a time obj based on that
// - then starts the timer using that object
void TimedTasks::dynamicTimer() {
  for (auto task : tasks) {
    if (task.callback) {
      timeObj _timer(task.timeMS);
      if (_timer.ding()) {
        task.callback();
        _timer.start();
      }
    }
  }
}

void TimedTasks::setTask(Tasks_t task) {
  tasks.push_back(task);
}
