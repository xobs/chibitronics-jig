#ifndef UNEXPORT_GPIO_H
#define UNEXPORT_GPIO_H
#include "chibitest.h"

class UnexportGpio : public ChibiTest
{
public:
    UnexportGpio(enum gpio);
    void runTest();

private:
    enum gpio gpio;
};

#endif // UNEXPORT_GPIO_H
