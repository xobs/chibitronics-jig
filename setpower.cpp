#include "setpower.h"

SetPower::SetPower(enum powerState onoff)
{
    name = "Set stickers power";
    state = onoff;
}

void SetPower::runTest()
{
    if (state == powerOn)
        testInfo("Turning power on");
    else
        testInfo("Turning power off");

    // Actually toggle power here
}
