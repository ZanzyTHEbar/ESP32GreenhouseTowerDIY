#pragma once

#include <helpers/id_interface.hpp>
#include <helpers/observer.hpp>
#include <memory>

class SensorInterface : public IId {
 public:
  virtual ~SensorInterface() = default;
  virtual void begin() = 0;
  virtual float read() = 0;
  virtual void subscribe(
      std::shared_ptr<Helpers::IObserver<float>> observer) = 0;
  virtual void unsubscribe(
      std::shared_ptr<Helpers::IObserver<float>> observer) = 0;
};