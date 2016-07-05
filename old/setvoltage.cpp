#include "setvoltage.h"

SetVoltage::SetVoltage(enum voltageLevel l)
{
    name = "Set stickers voltage";
    voltage = l;
}

void SetVoltage::runTest()
{
    if (voltage == fiveVolts) {
        testInfo("Setting 5V");
	setGpio(voltageGpio, 1);
    }
    else if (voltage == threeVolts) {
        testInfo("setting 3.3V");
	setGpio(voltageGpio, 0);
    }
    else
        testError("Unrecognized voltage");
}
