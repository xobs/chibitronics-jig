#include "delay.h"
#include <QString>
#include <QThread>

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};


Delay::Delay(int msecs)
{
    this->msecs = msecs;
    name = "Delay";
}

void Delay::runTest() {
    SleeperThread::msleep(msecs);
}
