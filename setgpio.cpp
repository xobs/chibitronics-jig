#include "setgpio.h"

SetGpio::SetGpio(enum gpio g, int v)
{
    name = "Set GPIO";
    gpio = g;
    value = v;
}

void SetGpio::runTest()
{
    setGpio(gpio, value);
}
