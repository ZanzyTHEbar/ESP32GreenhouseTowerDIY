#include "taskManager.hpp"

TaskManager::TaskManager(GreenHouseConfig* config) : deviceConfig(config) {}

TaskManager::~TaskManager() {}

void TaskManager::update(ObserverEvent::CustomEvents event) {}

//**********************************************************************************************************************
//*
//!                                                TaskHandler
//*
//**********************************************************************************************************************
void TaskManager::taskHandler() {
  for (auto itr = tasks.begin(); itr != tasks.end(); ++itr) {
    this->notify(*itr);
  }
}

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
                                  bool shouldNotify) {
  size_t size = deviceConfig->config.relays.size();
  if (size == 0) {
    log_i("No Relays found, adding new one");
    deviceConfig->config.relays.emplace_back(name, port);
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
  } else if (size < 100) {
    log_i("Relay not found, adding new one");
    deviceConfig->config.relays.emplace_back(name, port);
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
