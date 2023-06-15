#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <utilities/helpers.hpp>
#include "local/data/visitor.hpp"

template <typename T>
class SensorSerializer : public Visitor<SensorInterface<T>> {
 private:
  static const char* fmt;

 public:
  void visit(SensorInterface<T>* sensor) override {
    // T reading = static_cast<T>(sensor->read());
    serializedData.assign(Helpers::format_string(
        fmt, sensor->getSensorName().c_str(), sensor->read()));
    value = sensor->read();
    sensorName.assign(sensor->getSensorName());
  };

  std::string serializedData;
  std::string sensorName;
  T value;
};

#endif