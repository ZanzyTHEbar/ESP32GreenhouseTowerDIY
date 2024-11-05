#pragma once

template <typename T>
class StrategyInterface {
 public:
  virtual ~StrategyInterface() = default;
  virtual void execute(T strategy) = 0;
};