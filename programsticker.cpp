#include "programsticker.h"
#include <QString>

ProgramSticker::ProgramSticker(QString firmware, int num)
    : stickerNum(num), firmwareFile(firmware)
{
    name = "Program one sticker";
}

void ProgramSticker::runTest() {
    testInfo("Programming sticker...");
    emit testMessage(testName(), setStickerNum, stickerNum, "");
}
