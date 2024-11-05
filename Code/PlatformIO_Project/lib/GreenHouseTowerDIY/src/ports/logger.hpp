#pragma once

#include <string>

class LoggerInterface {
 public:
  virtual ~LoggerInterface() = default;
  virtual void log(const std::string& message) = 0;  // Logs a message
};