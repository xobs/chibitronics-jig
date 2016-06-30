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

    SetPower(const QHash<QString, QVariant> &settings = QHash<QString, QVariant>());
    void runTest();

private:
    enum powerState state;
};

#endif // SETPOWER_H
