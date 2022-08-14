#ifndef XMQTTBASECLASS_HPP
#define XMQTTBASECLASS_HPP
#include <defines.hpp>
#include "network/networkStack/network.hpp"
#include "network/ntp.hpp"
#include "io/Pump/pump.hpp"
#include "io/Relays/Relays.hpp"
#include "sensors/pH/pHsensor.hpp"
#include "data/StateManager/StateManager.hpp"
#include "data/AccumulateData/accumulatedata.hpp"

class XMqttBaseClass //: public Network
{
public:
    // Constructor
    XMqttBaseClass(PHSENSOR *_phSensor);
    virtual ~XMqttBaseClass();

    virtual bool begin();

    void loadMQTTConfig();
    void mqttLoop();

    // Handles messages arrived on subscribed topic(s)
    virtual void callback(const char *topic, byte *payload, unsigned int length);
    virtual void callback(const char *topic, const uint8_t *payload, uint16_t length);
    // Get's the broker address
    const char *getBrokerAddress();

    unsigned long lastReadAt;
    unsigned long lastAvailabilityToggleAt;
    bool lastInputState;
    unsigned long lastSentAt;

    const char *_infoTopic;
    const char *_statusTopic;
    const char *_commandTopic;
    const char *_configTopic;
    const char *_menuTopic;

    // Value-Defintions of the different String values
    enum RelayEnum
    {
        UnDefined,
        On,
        Off,
        End
    };

    enum PumpEnum
    {
        UnDefinedPump,
        PumpOn,
        PumpOff,
        PumpEnd
    };

    enum CallbackEnum
    {
        UnDefinedCallback,
        Pump,
        Relays,
        Ph,
        Info,
        Menu,
        EndCallback
    };

    static std::unordered_map<std::string, RelayEnum> s_relay_control_map;
    static std::unordered_map<std::string, PumpEnum> s_pump_control_map;
    static std::unordered_map<std::string, CallbackEnum> s_callback_map;

    PHSENSOR* _phSensor;
};
#endif // XMQTTBASECLASS_HPP