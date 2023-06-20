#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP
#include <Arduino.h>
#include <string>
#include <unordered_map>
#include <utilities/helpers.hpp>
#include "local/data/visitor.hpp"

template <typename T>
class SensorSerializer : public Visitor<SensorInterface<T>> {
 private:
  static const char* fmt;

 public:
  void visit(SensorInterface<T>* sensor) override {
    auto read = sensor->read();

    log_d("Serializing %s", sensor->getSensorName().c_str());
    serializedData.assign(
        Helpers::format_string(fmt, sensor->getSensorName().c_str(), read));
    value = read;
    sensorName.assign(sensor->getSensorName());
  };

  std::string serializedData;
  std::string sensorName;
  T value;
};

#endif