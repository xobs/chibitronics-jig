#ifndef SETVOLTAGE_H
#define SETVOLTAGE_H
#include "chibitest.h"

class SetVoltage : public ChibiTest
{
public:
    enum voltageLevel {
        threeVolts,
        fiveVolts,
    };
    SetVoltage(enum voltageLevel);
    void runTest();

private:
    enum voltageLevel voltage;
};

#endif // SETVOLTAGE_H
