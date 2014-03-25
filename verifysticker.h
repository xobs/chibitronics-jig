#ifndef VERIFYSTICKER_H
#define VERIFYSTICKER_H
#include "chibitest.h"

class VerifySticker : public ChibiTest
{
	Q_OBJECT
public:
    VerifySticker(int stickerNum, QString firmwareFile,
                   QString configFile = "chibi-sensor.conf",
                   QString partName = "attiny5");
    void runTest();

private:
    int stickerNum;
    QString firmwareFile;
    QString configFile;
    QString partName;
};

#endif // VERIFYSTICKER_H
