#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP
#include <local/data/config/config.hpp>
#include <utilities/Observer.hpp>

namespace ObserverEvent
{
  enum CustomEvents
  {
    relaysConfigChanged,
    relaysActivated,
    accumulateData,
    mqttConfigChanged,
    dhtConfigChanged,
  };
}
/**
 * @brief: This is a task handler for the background tasks
 * @note: Background task handler
 */
class TaskManager : public ISubject<ObserverEvent::CustomEvents>
{
public:
  TaskManager(GreenHouseConfig *config);
  virtual ~TaskManager();

  typedef std::set<ObserverEvent::CustomEvents> Tasks_t;
  void taskHandler();
  void setTask(ObserverEvent::CustomEvents task,
               IObserver<ObserverEvent::CustomEvents> *observer);
  void eraseTask(ObserverEvent::CustomEvents task,
                 IObserver<ObserverEvent::CustomEvents> *observer);
  void update(ObserverEvent::CustomEvents event);
  void detachAll();
  /* Set Methods */
  void setRelaysConfig(const std::string &name,
                       uint8_t port,
                       bool start_state,
                       timeObj *timer,
                       bool shouldNotify = true);

  void removeRelay(const std::string &name, bool shouldNotify = true);

  void setMQTTConfig(const std::string &broker,
                     uint16_t port,
                     const std::string &username = std::string(),
                     const std::string &password = std::string(),
                     bool shouldNotify = true);
  void setDHT(const std::string &type, const uint8_t pin, bool shouldNotify);

private:
  Tasks_t tasks;
  GreenHouseConfig *deviceConfig;
};

#endif
