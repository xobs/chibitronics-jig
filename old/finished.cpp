#include "finished.h"
#include <QString>

Finished::Finished()
{
    name = "Finished";
}

void Finished::runTest()
{
    emit testMessage(testName(), setStickerNum, 0, "");
}
