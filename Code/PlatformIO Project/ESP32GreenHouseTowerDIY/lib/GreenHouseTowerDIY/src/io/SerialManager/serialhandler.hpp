#include "serialmanager.hpp"
#include "io/Pump/pump.hpp"

class SerialHandler
{
public:
    SerialHandler();
    virtual ~SerialHandler();
    void begin(char *cmd, void (*callback)());
    void loop();
};