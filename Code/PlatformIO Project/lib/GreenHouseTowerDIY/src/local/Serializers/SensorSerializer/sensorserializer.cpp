#include "sensorserializer.hpp"
#include <string>
#include <unordered_map>

//* Format Specializers
template <>
const char* SensorSerializer<int>::fmt = "\"%s\":%d,";
template <>
const char* SensorSerializer<long>::fmt = "\"%s\":%ld,";
template <>
const char* SensorSerializer<float>::fmt = "\"%s\":%.3f,";

//* Specialize for std::string
template <>
void SensorSerializer<std::string>::visit(
    SensorInterface<std::string>* sensor) {
  serializedData.assign(Helpers::format_string("\"%s\":\"%s\",",
                                               sensor->getSensorName().c_str(),
                                               sensor->read().c_str()));
  value = sensor->read();
  sensorName.assign(sensor->getSensorName());
}

//* Specialize for float vectors
template <>
void SensorSerializer<std::vector<float>>::visit(
    SensorInterface<std::vector<float>>* sensor) {
  serializedData.assign(
      Helpers::format_string("\"%s\":[", sensor->getSensorName().c_str()));
  for (auto& item : sensor->read()) {
    serializedData.append(Helpers::format_string("\"%.3f\",", item));
  }
  serializedData.append("],");
  value = sensor->read();
}

//* Specialize for string vectors
template <>
void SensorSerializer<std::vector<std::string>>::visit(
    SensorInterface<std::vector<std::string>>* sensor) {
  serializedData.assign(
      Helpers::format_string("\"%s\":[", sensor->getSensorName().c_str()));
  for (auto& item : sensor->read()) {
    serializedData.append(Helpers::format_string("\"%s\",", item.c_str()));
  }
  serializedData.append("],");
  value = sensor->read();
}

//* Specialize for unordered_map<string, float>
template <>
void SensorSerializer<std::unordered_map<std::string, float>>::visit(
    SensorInterface<std::unordered_map<std::string, float>>* sensor) {
  serializedData.assign(
      Helpers::format_string("\"%s\":{", sensor->getSensorName().c_str()));
  for (auto& item : sensor->read()) {
    serializedData.append(Helpers::format_string(
        "\"%s\":\"%.3f\",", item.first.c_str(), item.second));
  }
  serializedData.append("},");
  value = sensor->read();
}
