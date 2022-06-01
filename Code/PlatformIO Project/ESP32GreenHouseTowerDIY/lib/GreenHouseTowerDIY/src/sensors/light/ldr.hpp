#ifndef LDR_HPP
#define LDR_HPP
#include <defines.hpp>
#define LDR_PIN 33
class LDR
{
public:
    LDR();
    virtual ~LDR();
    float getLux();

private:
    const float _GAMMA;
    const float _RL10;
};
extern LDR ldr;
#endif