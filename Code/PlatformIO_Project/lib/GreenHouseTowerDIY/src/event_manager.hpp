#pragma once

#include <helpers/observer.hpp>

enum class EventState {
  Idle,
  Monitoring,
  Watering,
  Error,
};

enum class EventType {
  SensorTriggered,
  LowWaterLevel,
  TimeElapsed,
  ErrorDetected,
  WateringComplete,
  StateChanged,
  Reset,
};

class EventManager : public Helpers::ISubject<EventType> {
  EventState currentState;

  EventManager() : currentState(EventState::Idle) {}

  EventState getCurrentState() const { return currentState; }

  void setState(EventState newState) {
    if (currentState == newState) {
      return;
    }

    currentState = newState;
    notifyAll(EventType::StateChanged);
  }

  void handleEvent(EventType event) {
    switch (currentState) {
      case EventState::Idle:
        if (event == EventType::SensorTriggered) {
          setState(EventState::Monitoring);
        }
        break;

      case EventState::Monitoring:
        if (event == EventType::LowWaterLevel) {
          setState(EventState::Watering);
        } else if (event == EventType::ErrorDetected) {
          setState(EventState::Error);
        }
        break;

      case EventState::Watering:
        if (event == EventType::WateringComplete) {
          setState(EventState::Idle);
        }
        break;

      case EventState::Error:
        if (event == EventType::Reset) {
          setState(EventState::Idle);
        }
        break;
    }
  }

  void subscribe(std::shared_ptr<Helpers::IObserver<EventType>> observer) {
    attach(observer);
  }

  void unsubscribe(std::shared_ptr<Helpers::IObserver<EventType>> observer) {
    detach(observer);
  }

 public:
  void dispatchEvent(EventType event) {
    notifyAll(event);  // Notify all registered observers of this event type
  }
};