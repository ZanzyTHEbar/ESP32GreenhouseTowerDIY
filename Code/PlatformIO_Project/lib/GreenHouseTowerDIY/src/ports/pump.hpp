#pragma once

class PumpInterface {
 public:
  virtual ~PumpInterface() = default;
  virtual void activate() = 0;    // Activates the pump
  virtual void deactivate() = 0;  // Deactivates the pump
};