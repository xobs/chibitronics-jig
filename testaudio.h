#ifndef TESTAUDIO_H
#define TESTAUDIO_H
#include "chibitest.h"

class TestAudio : public ChibiTest
{
	Q_OBJECT
public:
    TestAudio(int equalizationMsecs);
    void runTest();

private:
    int equalizationMsecs;
};

#endif // TESTAUDIO_H
