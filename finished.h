#ifndef FINISHED_H
#define FINISHED_H
#include "chibitest.h"

class Finished : public ChibiTest
{
	Q_OBJECT
public:
    Finished();
    void runTest();

private:
    QString message;
};

#endif // FINISHED_H
