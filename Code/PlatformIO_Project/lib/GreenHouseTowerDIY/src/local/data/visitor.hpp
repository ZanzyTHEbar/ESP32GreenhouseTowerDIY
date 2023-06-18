#ifndef VISITOR_HPP
#define VISITOR_HPP

template <typename T>
class SensorInterface {
 public:
  virtual const std::string& getSensorName() = 0;
  virtual T read() = 0;
};

template <typename T>
class Element {
 public:
  virtual void accept(T& visitor) = 0;
};

template <typename T>
class Visitor {
 public:
  virtual void visit(T* element) = 0;
};

#endif