
#include "timedtasks.hpp"

TimedTasks::TimedTasks(void) : linkList()
{
}

TimedTasks::~TimedTasks(void)
{
}

bool TimedTasks::begin(void)
{
  _Timer_1s.setTime(1000);
  _Timer_5s.setTime(5000);
  _Timer_5s_2.setTime(5000);
  _Timer_10s.setTime(10000);
  _Timer_10s_2.setTime(10000);
  _Timer_30s.setTime(30000);
  _Timer_1m.setTime(60000);
  _Timer_5m.setTime(300000);
  return true;
}

#if ENABLE_I2C_SCANNER
void TimedTasks::ScanI2CBus(void)
{
  if (ENABLE_I2C_SCANNER)
  {
    if (_Timer_5s.ding())
    {
      Scan.SetupScan();
      Scan.BeginScan();
      _Timer_5s_2.start();
    }
  }
  else
  {
    return;
  }
}
#endif // ENABLE_I2C_SCANNER

void TimedTasks::accumulateSensorData(void)
{
  if (_Timer_1s.ding())
  {
    accumulatedata.InitAccumulateData();
    _Timer_1s.start();
  }
}

void TimedTasks::NTPService(void)
{
  if (_Timer_1s.ding())
  {
    networkntp.NTPLoop();
    _Timer_1s.start();
  }
}

void TimedTasks::checkNetwork(void)
{
  if (_Timer_10s.ding())
  {
    network.CheckNetworkLoop();
    _Timer_10s.start();
  }
}

void TimedTasks::updateCurrentData(void) // check to see if the data has changed
{
  if (_Timer_10s_2.ding())
  {
    cfg.updateCurrentData();
    log_i("Heap: %d", ESP.getFreeHeap());
    _Timer_10s_2.start();
  }
}

void TimedTasks::checkMQTTState(void) // check to see if the data has changed
{
  if (_Timer_10s_2.ding())
  {
#if ENABLE_HASS
    hassmqtt.checkState();
#else
    basemqtt.checkState();
#endif // ENABLE_HASS
    _Timer_10s_2.start();
  }
}

// Run down the list and call the idle() method on each one.
//! TODO: Turn this into a template that can accept any class type.
void TimedTasks::execute(void)
{
  AccumulateData *trace;

  trace = (AccumulateData *)getFirst();
  while (trace != NULL)
  {
    trace->execute();
    trace = (AccumulateData *)trace->getNext();
  }
}

TimedTasks timedTasks;