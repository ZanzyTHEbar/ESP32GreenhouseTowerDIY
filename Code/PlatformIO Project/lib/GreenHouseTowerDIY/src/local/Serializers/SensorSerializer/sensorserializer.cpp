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
  serializedData.assign(Helpers::format_string("\"%s\":\"%s\"",
                                               sensor->getSensorName().c_str(),
                                               sensor->read().c_str()));
  value = sensor->read();
  sensorName.assign(sensor->getSensorName());
}

//* Specialize for float vectors
template <>
void SensorSerializer<std::vector<float>>::visit(
    SensorInterface<std::vector<float>>* sensor) {
  std::string specifier_multi = "\"%.3f\",";
  std::string specifier_single = "\"%.3f\"";

  serializedData.assign(
      Helpers::format_string("\"%s\":[", sensor->getSensorName().c_str()));
  // check if the vector is of size 1 or if we are at the last element - and use
  // iterators instead of index
  for (auto it = sensor->read().begin(); it != sensor->read().end(); ++it) {
    serializedData.append(Helpers::format_string(
        (sensor->read().size() == 1 || it == sensor->read().end())
            ? specifier_single
            : specifier_multi,
        *it));
  }
  serializedData.append("]");
  value = sensor->read();
}

//* Specialize for string vectors
template <>
void SensorSerializer<std::vector<std::string>>::visit(
    SensorInterface<std::vector<std::string>>* sensor) {
  std::string specifier_multi = "\"%s\",";
  std::string specifier_single = "\"%s\"";

  serializedData.assign(
      Helpers::format_string("\"%s\":[", sensor->getSensorName().c_str()));
  // iterators instead of index
  for (auto it = sensor->read().begin(); it != sensor->read().end(); ++it) {
    serializedData.append(Helpers::format_string(
        (sensor->read().size() == 1 || it == sensor->read().end())
            ? specifier_single
            : specifier_multi,
        *it));
  }
  serializedData.append("]");
  value = sensor->read();
}

//* Specialize for unordered_map<string, float>
template <>
void SensorSerializer<std::unordered_map<std::string, float>>::visit(
    SensorInterface<std::unordered_map<std::string, float>>* sensor) {
  std::string specifier_multi = "\"%s\":\"%.3f\",";
  std::string specifier_single = "\"%s\":\"%.3f\"";

  serializedData.assign(
      Helpers::format_string("\"%s\":{", sensor->getSensorName().c_str()));
  // check if the vector is of size 1 or if we are at the last element - and use
  // iterators instead of index
  for (auto it = sensor->read().begin(); it != sensor->read().end(); ++it) {
    serializedData.append(Helpers::format_string(
        (sensor->read().size() == 1 || it == sensor->read().end() )
            ? specifier_single
            : specifier_multi,
        it->first.c_str(), it->second));
  }
  serializedData.append("}");
  value = sensor->read();
}
