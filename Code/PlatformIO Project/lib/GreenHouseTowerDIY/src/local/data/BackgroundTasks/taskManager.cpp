#include "taskManager.hpp"

TaskManager::TaskManager(GreenHouseConfig* config) : deviceConfig(config) {}

TaskManager::~TaskManager() {
  this->detachAll();
}

void TaskManager::update(ObserverEvent::CustomEvents event) {}

//**********************************************************************************************************************
//*
//!                                                TaskHandler
//*
//**********************************************************************************************************************

/**
 * @brief Handles the tasks that are set by the user
 * @note This function is to be called in the main loop
 */
void TaskManager::taskHandler() {
  for (auto itr = tasks.begin(); itr != tasks.end(); ++itr) {
    this->notify(*itr);
  }
}

/**
 * @brief Sets a task to be handled by the task manager
 * @note This function is to be called in the setup
 * @param task The task to be handled
 * @param observer The observer that will handle the task
 */
void TaskManager::setTask(ObserverEvent::CustomEvents task,
                          IObserver<ObserverEvent::CustomEvents>* observer) {
  tasks.insert(task);
  this->attach(observer);
}

void TaskManager::eraseTask(ObserverEvent::CustomEvents task,
                            IObserver<ObserverEvent::CustomEvents>* observer) {
  tasks.erase(task);
  this->detach(observer);
}

//**********************************************************************************************************************
//*
//!                                                SetMethods
//*
//**********************************************************************************************************************

void TaskManager::setRelaysConfig(const std::string& name,
                                  uint8_t port,
                                  bool start_state,
                                  timeObj* timer,
                                  bool shouldNotify = true) {
  size_t size = deviceConfig->config.relays.size();
  if (size == 0) {
    log_i("No Relays found, adding new one");
    deviceConfig->config.relays.emplace_back(name, port, start_state, time);
  }
  int relayToUpdate = -1;
  for (size_t i = 0; i < size; i++) {
    if (strcmp(deviceConfig->config.relays[i].name.c_str(), name.c_str()) ==
        0) {
      relayToUpdate = i;
      break;
    }
  }
  if (relayToUpdate >= 0) {
    log_i("Relay found, updating");
    deviceConfig->config.relays[relayToUpdate].port = port;
    deviceConfig->config.relays[relayToUpdate].start_state = start_state;
    deviceConfig->config.relays[relayToUpdate].timer = timer;
  } else if (size < 100) {
    log_i("Relay not found, adding new one");
    deviceConfig->config.relays.emplace_back(name, port, start_state, time);
  } else {
    log_e("Relay not found, max amount of relays (100) reached");
  }
  if (shouldNotify) {
    this->notify(ObserverEvent::relaysConfigChanged);
  }
}

void TaskManager::setMQTTConfig(const std::string& broker,
                                uint16_t port,
                                const std::string& username,
                                const std::string& password,
                                bool shouldNotify) {}

void TaskManager::detachAll() {
  ISubject::detachAll();
  tasks.clear();
}
