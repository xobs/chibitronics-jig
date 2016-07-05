#ifndef TESTLED_H
#define TESTLED_H
#include "chibitest.h"

class TestLed : public ChibiTest
{
	Q_OBJECT
public:
    TestLed(int threshold);
    void runTest();

private:
    int threshold;
};

#endif // TESTLED_H
