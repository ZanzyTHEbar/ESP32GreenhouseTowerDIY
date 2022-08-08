#include "serialmanager.hpp"
#include "io/Pump/pump.hpp"
#include "sensors/pH/pHsensor.hpp"

class SerialHandler
{
public:
    SerialHandler();
    virtual ~SerialHandler();
    void begin();
    void loop();
};