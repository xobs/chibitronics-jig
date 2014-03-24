#include "setpower.h"

SetPower::SetPower(enum powerState onoff)
{
    name = "Set stickers power";
    state = onoff;
}

void SetPower::runTest()
{
    if (state == powerOn) {
        testInfo("Turning power on");
        setGpio(59, 1);
    }
    else {
        testInfo("Turning power off");
        setGpio(59, 0);
    }
}
