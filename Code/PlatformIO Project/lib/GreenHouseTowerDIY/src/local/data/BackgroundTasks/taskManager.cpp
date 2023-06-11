#include "taskManager.hpp"

TaskManager::TaskManager(GreenHouseConfig& config) : deviceConfig(config) {}

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
    this->notifyAll(*itr);
  }
}

/**
 * @brief Sets a task to be handled by the task manager
 * @note This function is to be called in the setup
 * @param task The task to be handled
 * @param observer The observer that will handle the task
 */
void TaskManager::setTask(ObserverEvent::CustomEvents task,
                          IObserver<ObserverEvent::CustomEvents>& observer) {
  tasks.insert(task);
  this->attach(observer);
}

void TaskManager::eraseTask(ObserverEvent::CustomEvents task,
                            IObserver<ObserverEvent::CustomEvents>& observer) {
  tasks.erase(task);
  this->detach(observer);
}

void TaskManager::detachAll() {
  ISubject::detachAll();
  tasks.clear();
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
                                  HASwitch* ha_switch,
                                  bool shouldNotify) {
  size_t size = deviceConfig.config.relays.size();
  if (size == 0) {
    log_i("No Relays found, adding new one");
    deviceConfig.config.relays.emplace_back(name, port, start_state, timer,
                                            ha_switch);
  } else {
    int relayToUpdate = -1;
    for (size_t i = 0; i < size; i++) {
      if (deviceConfig.config.relays[i].name == name) {
        relayToUpdate = i;
        break;
      }
    }
    if (relayToUpdate >= 0) {
      log_i("Relay found, updating");
      deviceConfig.config.relays[relayToUpdate].port = port;
      deviceConfig.config.relays[relayToUpdate].start_state = start_state;
      deviceConfig.config.relays[relayToUpdate].timer = timer;
      deviceConfig.config.relays[relayToUpdate].ha_switch = ha_switch;
    } else if (size < 100) {
      log_i("Relay not found, adding new one");
      deviceConfig.config.relays.emplace_back(name, port, start_state, timer,
                                              ha_switch);
    } else {
      log_e("Relay not found, max amount of relays (100) reached");
    }
  }

  if (shouldNotify) {
    this->notify(ObserverEvent::relaysConfigChanged, name);
  }
}

void TaskManager::removeRelay(const std::string& name, bool shouldNotify) {
  size_t size = deviceConfig.config.relays.size();
  if (size == 0) {
    log_i("No Relays found, nothing to remove");
  } else {
    int relayToRemove = -1;
    for (size_t i = 0; i < size; i++) {
      if (deviceConfig.config.relays[i].name == name) {
        relayToRemove = i;
        break;
      }
    }
    if (relayToRemove >= 0) {
      log_i("Relay found, removing");
      // set the relay to off
      digitalWrite(deviceConfig.config.relays[relayToRemove].port, LOW);
      // remove the relay from the vector
      deviceConfig.config.relays.erase(deviceConfig.config.relays.begin() +
                                       relayToRemove);
    } else {
      log_e("Relay not found, nothing to remove");
    }
  }

  if (shouldNotify) {
    this->notify(ObserverEvent::relaysConfigChanged, name);
  }
}

void TaskManager::setMQTTConfig(const std::string& broker,
                                uint16_t port,
                                const std::string& username,
                                const std::string& password,
                                bool shouldNotify) {
  deviceConfig.config.mqtt.broker.assign(broker);
  deviceConfig.config.mqtt.port = port;

  if ((username.length() || password.length()) > 0) {
    deviceConfig.config.mqtt.username.assign(username);
    deviceConfig.config.mqtt.password.assign(password);
  }

  if (shouldNotify) {
    this->notifyAll(ObserverEvent::mqttConfigChanged);
  }
}

void TaskManager::setDHT(const std::string& type,
                         const uint8_t pin,
                         bool shouldNotify) {
  deviceConfig.config.enabled_features.dht_type.assign(type);
  deviceConfig.config.enabled_features.dht_pin = pin;

  if (shouldNotify) {
    this->notifyAll(ObserverEvent::dhtConfigChanged);
  }
}
