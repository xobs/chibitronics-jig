#ifndef SETSTICKERFUSE_H
#define SETSTICKERFUSE_H
#include "chibitest.h"

class SetStickerFuse : public ChibiTest
{
	Q_OBJECT
public:
    SetStickerFuse(int stickerNum, QString fuseName, int fuseValue,
                    QString configFile = "chibi-sensor.conf",
                    QString partName = "attiny5");
    void runTest();

private:
    int stickerNum;
    int fuseValue;
    QString fuseName;
    QString configFile;
    QString partName;
};

#endif // SETSTICKERFUSE_H
