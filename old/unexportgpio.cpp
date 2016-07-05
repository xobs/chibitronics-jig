#include "unexportgpio.h"

UnexportGpio::UnexportGpio(enum gpio g)
{
    name = "Unexport GPIO";
    gpio = g;
}

void UnexportGpio::runTest()
{
    unexportGpio(gpio);
}
