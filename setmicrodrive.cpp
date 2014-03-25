#include "setmicrodrive.h"

SetMicroDrive::SetMicroDrive(enum microDrive m)
{
    name = "Set microcontrolelr drive";
    microDrive = m;
}

void SetMicroDrive::runTest()
{
    if (microDrive == program) {
        testInfo("Setting Microcontroller to program");
        setGpio(microDriveGpio, 0);
    }
    else if (microDrive == execute) {
        testInfo("Setting Microcontroller to run");
        setGpio(microDriveGpio, 1);
        setGpio(tpiDatGpio, 0);
        setGpio(tpiSignalGpio, 0);
    }
    else
        testError("Unrecognized microcontroller drive value");
}
