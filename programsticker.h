#ifndef PROGRAMSTICKER_H
#define PROGRAMSTICKER_H
#include "chibitest.h"

class ProgramSticker : public ChibiTest
{
	Q_OBJECT
public:
    ProgramSticker(QString firmwareFile, int stickerNum);
    void runTest();

private:
    int stickerNum;
    QString firmwareFile;
};

#endif // PROGRAMSTICKER_H
