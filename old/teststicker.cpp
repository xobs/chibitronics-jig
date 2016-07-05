#include "teststicker.h"
#include <QString>

TestSticker::TestSticker(enum stickerType t, int num)
    : stickerNum(num), stickerType(t)
{
    name = "Test one sticker";
}

void TestSticker::runTest() {
    /* 1: Twinkle */
    /* 2: Heartbeat */
    /* 3: Blink */
    /* 4: Fade */
    int i;
    emit testMessage(testName(), infoMessage, stickerNum,
            QString("Testing sticker ") + QString::number(stickerNum));

    selectSticker(stickerNum);

    int count = 0;
    for (i = 0; i < 100; i++) {
        int g = getGpio(54);
        count += g;
        msleep(10);
    }

    if (count > 90)
        testError(QString() 
                + "Sticker " + QString::number(stickerNum) + " stuck high");
    else if (count < 10)
        testError(QString() 
                + "Sticker " + QString::number(stickerNum) + " stuck low");
    else
        emit testMessage(testName(), testPass, stickerNum, "Test passed");
}
