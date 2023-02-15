#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP
#include <local/data/config/config.hpp>
#include <utilities/Observer.hpp>

namespace ObserverEvent {
  enum CustomEvents { relaysConfigChanged };
}
/**
 * @brief: This is a task handler for the background tasks
 * @note: Background task handler
 */
class TaskManager : public ISubject<ObserverEvent::CustomEvents> {
 public:
  TaskManager(GreenHouseConfig* config);
  virtual ~TaskManager();

  typedef std::set<ObserverEvent::CustomEvents> Tasks_t;
  void taskHandler();
  void setTask(ObserverEvent::CustomEvents task,
               IObserver<ObserverEvent::CustomEvents>* observer);
  void eraseTask(ObserverEvent::CustomEvents task,
                 IObserver<ObserverEvent::CustomEvents>* observer);
  void update(ObserverEvent::CustomEvents event);

  /* Set Methods */
  void setRelaysConfig(const std::string& name,
                       uint8_t port,
                       bool start_state,
                       timeObj* timer,
                       bool shouldNotify = true);

  void setMQTTConfig(const std::string& broker,
                     uint16_t port,
                     const std::string& username,
                     const std::string& password,
                     bool shouldNotify = true);

 private:
  Tasks_t tasks;
  GreenHouseConfig* deviceConfig;
};

#endif
