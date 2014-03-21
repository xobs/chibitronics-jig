#ifndef SETPOWER_H
#define SETPOWER_H
#include <QThread>
#include <QFile>
#include "chibitest.h"

struct usb_descr;

class SetPower : public ChibiTest
{
public:
    enum powerState {
        powerOn,
        powerOff,
    };
    SetPower(enum powerState);
    void runTest();

private:
    enum powerState state;
};

#endif // SETPOWER_H
