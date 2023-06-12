#include "sensorserializer.hpp"

SensorSerializer::SensorSerializer() : serializedData(std::string()) {}

void SensorSerializer::visit(SensorInterface<float>* sensor) {
  serializedData.assign(Helpers::format_string(
      "\"%s\":%f,", sensor->getSensorName().c_str(), sensor->read()));
}

StringSerializer::StringSerializer() : serializedData(std::string()) {}

void StringSerializer::visit(SensorInterface<const std::string&>* sensor) {
  serializedData.assign(Helpers::format_string("\"%s\":\"%s\",",
                                               sensor->getSensorName().c_str(),
                                               sensor->read().c_str()));
}