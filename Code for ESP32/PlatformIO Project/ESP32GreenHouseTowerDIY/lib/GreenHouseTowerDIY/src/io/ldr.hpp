#ifndef LDR_HPP
#define LDR_HPP
#include <defines.hpp>
#define LDR_PIN 33
class LDR
{
public:
    LDR();
    virtual ~LDR();
    void setupLDR();
    void loopLDR();
    float getLux();

private:
};
extern LDR ldr;
#endif