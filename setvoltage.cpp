#include "setvoltage.h"

SetVoltage::SetVoltage(enum voltageLevel l)
{
    name = "Set stickers voltage";
    voltage = l;
}

void SetVoltage::runTest()
{
    if (voltage == fiveVolts)
        testInfo("Setting 5V");
    else if (voltage == threeVolts)
        testInfo("setting 3.3V");
    else
        testError("Unrecognized voltage");

    // Actually toggle voltage here
}
