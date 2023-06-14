#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <string>
#include <type_traits>
#include <utilities/helpers.hpp>
#include "local/data/visitor.hpp"

template <typename T>
class SensorSerializer : public Visitor<SensorInterface<T>> {
 public:
  void visit(SensorInterface<T>* sensor) override {
    if constexpr (std::is_same_v<T, const std::string&>) {
      serializedData.assign(Helpers::format_string(
          "\"%s\":\"%s\",", sensor->getSensorName().c_str(),
          sensor->read().c_str()));
      value.assign(sensor->read());
      return;
    }

    if constexpr (std::is_same_v<T, std::vector<std::string>>) {
      serializedData.assign(
          Helpers::format_string("\"%s\":[", sensor->getSensorName().c_str()));
      for (auto& item : sensor->read()) {
        serializedData.append(Helpers::format_string("\"%s\",", item.c_str()));
      }
      serializedData.append("],");
      value = sensor->read();
      return;
    }

    if constexpr (std::is_same_v<T, std::vector<float>>) {
      serializedData.assign(
          Helpers::format_string("\"%s\":[", sensor->getSensorName().c_str()));
      for (auto& item : sensor->read()) {
        serializedData.append(Helpers::format_string("\"%.3f\",", item));
      }
      serializedData.append("],");
      value = sensor->read();
      return;
    }

    if constexpr (std::is_same_v<T, std::unordered_map<std::string, float>>) {
      serializedData.assign(
          Helpers::format_string("\"%s\":{", sensor->getSensorName().c_str()));
      for (auto& item : sensor->read()) {
        serializedData.append(Helpers::format_string(
            "\"%s\":%.3f,", item.first.c_str(), item.second));
      }
      serializedData.append("},");
      value = sensor->read();
      return;
    }

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