#include "teststicker.h"
#include <QString>

TestSticker::TestSticker(enum stickerType t, int num)
    : stickerNum(num), stickerType(t)
{
    name = "Test one sticker";
}

void TestSticker::runTest() {
    testInfo("Testing sticker...");
    emit testMessage(testName(), setStickerNum, stickerNum, "");
}
