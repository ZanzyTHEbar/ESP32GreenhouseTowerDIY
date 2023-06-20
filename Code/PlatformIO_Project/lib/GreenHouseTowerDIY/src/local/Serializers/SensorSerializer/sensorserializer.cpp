#include "sensorserializer.hpp"
#include <string>
#include <unordered_map>

//* Format Specializers
template <>
const char* SensorSerializer<int>::fmt = "\"%s\":%d";
template <>
const char* SensorSerializer<long>::fmt = "\"%s\":%ld";
template <>
const char* SensorSerializer<float>::fmt = "\"%s\":%.3f";

//* Specialize for std::string
template <>
void SensorSerializer<std::string>::visit(
    SensorInterface<std::string>* sensor) {
  auto read = sensor->read();

  serializedData.assign(Helpers::format_string(
      "\"%s\":\"%s\"", sensor->getSensorName().c_str(), read.c_str()));
  value = read;
  sensorName.assign(sensor->getSensorName());
}

//* Specialize for float vectors
template <>
void SensorSerializer<std::vector<float>>::visit(
    SensorInterface<std::vector<float>>* sensor) {
  auto read = sensor->read();

  serializedData.assign(
      Helpers::format_string("\"%s\":[", sensor->getSensorName().c_str()));
  for (auto&& value : read) {
    serializedData.append(Helpers::format_string("%.3f,", value));
  }
  // remove the last comma
  serializedData.pop_back();
  serializedData.append("]");
  sensorName.assign(sensor->getSensorName());
  value = read;
}

//* Specialize for string vectors
template <>
void SensorSerializer<std::vector<std::string>>::visit(
    SensorInterface<std::vector<std::string>>* sensor) {
  auto read = sensor->read();

  serializedData.assign(
      Helpers::format_string("\"%s\":[", sensor->getSensorName().c_str()));
  for (auto&& value : read) {
    serializedData.append(Helpers::format_string("\"%s\",", value.c_str()));
  }
  // remove the last comma
  serializedData.pop_back();
  serializedData.append("]");
  sensorName.assign(sensor->getSensorName());
  value = read;
}

//* Specialize for unordered_map<string, float>
template <>
void SensorSerializer<std::unordered_map<std::string, float>>::visit(
    SensorInterface<std::unordered_map<std::string, float>>* sensor) {
  auto read = sensor->read();

  serializedData.assign(
      Helpers::format_string("\"%s\":{", sensor->getSensorName().c_str()));

  for (auto&& kv : read) {
    serializedData.append(Helpers::format_string("\"%s\":%.3f,",
                                                 kv.first.c_str(), kv.second));
  }

  // remove the last comma
  serializedData.pop_back();
  serializedData.append("}");
  sensorName.assign(sensor->getSensorName());
  value = read;
}
