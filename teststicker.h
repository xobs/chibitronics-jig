#ifndef TESTSTICKER_H
#define TESTSTICKER_H
#include "chibitest.h"

class TestSticker : public ChibiTest
{
	Q_OBJECT
public:
    enum stickerType {
        twinkleSticker,
        heartbeatSticker,
        blinkSticker,
        fadeSticker,
    };
    TestSticker(enum stickerType type, int stickerNum);
    void runTest();

private:
    int stickerNum;
    enum stickerType stickerType;
};

#endif // TESTSTICKER_H
