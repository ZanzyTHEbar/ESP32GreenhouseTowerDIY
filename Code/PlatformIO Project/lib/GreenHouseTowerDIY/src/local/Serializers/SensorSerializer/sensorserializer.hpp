#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <string>

#include <utilities/helpers.hpp>
#include "local/data/visitor.hpp"

class SensorSerializer : public Visitor<SensorInterface<float>> {
 public:
  SensorSerializer();
  void visit(SensorInterface<float>* sensor) override;

  std::string serializedData;
};

#endif