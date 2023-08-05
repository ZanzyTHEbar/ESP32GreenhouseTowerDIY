#ifndef VISITOR_HPP
#define VISITOR_HPP

template <typename T>
class SensorInterface {
 public:
  virtual ~SensorInterface() = default;
  virtual const std::string& getSensorName() = 0;
  virtual T read() = 0;
};

template <typename T>
class Element {
 public:
  virtual ~Element() = default;
  virtual void accept(T& visitor) = 0;
};

template <typename T>
class Visitor {
 public:
  virtual ~Visitor() = default;
  virtual void visit(T* element) = 0;
};

#endif