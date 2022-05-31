
#include "timedtasks.hpp"

TimedTasks::TimedTasks()
{
}

TimedTasks::~TimedTasks()
{
}

void TimedTasks::setupTimers()
{
  Timer_1s.setTime(1000);
  Timer_5s.setTime(5000);
  Timer_5s_2.setTime(5000);
  Timer_10s.setTime(10000);
  Timer_10s_2.setTime(10000);
  Timer_30s.setTime(30000);
  Timer_1m.setTime(60000);
  Timer_5m.setTime(300000);
}

void TimedTasks::ScanI2CBus()
{
  if (ENABLE_I2C_SCANNER)
  {
    if (Timer_5s.ding())
    {
      Scan.SetupScan();
      Scan.BeginScan();
      Timer_5s_2.start();
    }
  }
  else
  {
    return;
  }
}

void TimedTasks::accumulateSensorData()
{
  if (Timer_1s.ding())
  {
    accumulatedata.InitAccumulateData();
    Timer_1s.start();
  }
}

void TimedTasks::accumulateSensorData()
{
  if (Timer_1s.ding())
  {
    networkntp.NTPLoop();
    Timer_1s.start();
  }
}

void TimedTasks::checkNetwork()
{
  if (Timer_10s.ding())
  {
    network.CheckNetworkLoop();
    Timer_10s.start();
  }
}

void TimedTasks::updateCurrentData() // check to see if the data has changed
{
  if (Timer_10s_2.ding())
  {
    cfg.updateCurrentData();
    log_i("Heap: %d", ESP.getFreeHeap());
    Timer_10s_2.start();
  }
}

// Timer delay Settings
/* void TimedTasks::setCallback(void (*funct)())
{
  callback = funct;
  hookup();
}

void TimedTasks::setSeconds(float seconds) { setTime(seconds * 1000); }

void TimedTasks::idle()
{
  if (ding())
  {
    stepTime();
    callback();
  }
} */

TimedTasks timedTasks;