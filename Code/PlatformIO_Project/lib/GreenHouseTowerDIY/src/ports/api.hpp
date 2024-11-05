#pragma once

#include <string>

// FIXME: Implement a concrete Message Type to send and receive messages,
// instead of a string
class APIInterface {
 public:
  virtual ~APIInterface() = default;
  virtual void send(const std::string& message) = 0;  // Sends a message
  virtual std::string receive() = 0;                  // Receives a message
};