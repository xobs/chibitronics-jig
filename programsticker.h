#ifndef PROGRAMSTICKER_H
#define PROGRAMSTICKER_H
#include "chibitest.h"

class ProgramSticker : public ChibiTest
{
	Q_OBJECT
public:
    ProgramSticker(int stickerNum, QString firmwareFile,
                   QString configFile = "chibi-sensor.conf",
                   QString partName = "attiny5");
    void runTest();

private:
    int stickerNum;
    QString firmwareFile;
    QString configFile;
    QString partName;
};

#endif // PROGRAMSTICKER_H
