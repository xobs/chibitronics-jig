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
        setGpio(powerGpio, 1);
    }
    else {
        testInfo("Turning power off");
        setGpio(powerGpio, 0);
    }
}
