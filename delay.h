#ifndef DELAY_H
#define DELAY_H
#include "chibitest.h"

class Delay : public ChibiTest
{
	Q_OBJECT
public:
    Delay(int msecs);
    void runTest();

private:
    int msecs;
};

#endif // DELAY_H
