#include "delay.h"

Delay::Delay(int msecs)
{
    this->msecs = msecs;
    name = "Delay";
}

void Delay::runTest() {
    msleep(msecs);
}
