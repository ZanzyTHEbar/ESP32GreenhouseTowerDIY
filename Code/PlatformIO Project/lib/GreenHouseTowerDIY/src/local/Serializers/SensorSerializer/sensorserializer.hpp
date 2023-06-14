#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <string>
#include <type_traits>
#include <unordered_map>
#include <utilities/helpers.hpp>
#include "local/data/visitor.hpp"

template <typename T>
class SensorSerializer : public Visitor<SensorInterface<T>> {
 public:
  void visit(SensorInterface<T>* sensor) override {
    serializedData.assign(Helpers::format_string(
        "\"%s\":%.3f,", sensor->getSensorName().c_str(), sensor->read()));
    value = sensor->read();
    sensorName.assign(sensor->getSensorName());
  };

  std::string serializedData;
  std::string sensorName;
  T value;
};

#endif