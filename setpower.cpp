#include "setpower.h"

SetPower::SetPower(const QHash<QString, QVariant> &settings)
  : ChibiTest(settings)
{
    name = "Set stickers power";
    if (settings["power"].toString() == "on")
      state = powerOn;
    else
      state = powerOff;
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
