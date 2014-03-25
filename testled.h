#ifndef TESTLED_H
#define TESTLED_H
#include "chibitest.h"

class TestLed : public ChibiTest
{
	Q_OBJECT
public:
    TestLed(void);
    void runTest();

private:
};

#endif // TESTLED_H
