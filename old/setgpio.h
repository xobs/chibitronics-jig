#ifndef SET_GPIO_H
#define SET_GPIO_H
#include "chibitest.h"

class SetGpio : public ChibiTest
{
public:
    SetGpio(enum gpio, int value);
    void runTest();

private:
    enum gpio gpio;
    int value;
};

#endif // SET_GPIO_H
