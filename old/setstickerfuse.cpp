#include <QString>
#include <QProcess>
#include "setstickerfuse.h"

SetStickerFuse::SetStickerFuse(int num, QString fuse, int value,
                               QString config, QString part)
    : stickerNum(num), fuseValue(value), fuseName(fuse),
      configFile(config), partName(part)
{
    name = "Set sticker fuse";
}

void SetStickerFuse::runTest()
{
    QProcess avrdude;

    emit testMessage(testName(), infoMessage, stickerNum, 
            QString("Setting fuse ") + fuseName 
            + " to 0x" + QString::number(fuseValue, 16));

    selectSticker(stickerNum);

    avrdude.start("./avrdude", QStringList()
            << "-C" << configFile
            << "-c" << "linuxgpio"
            << "-P" << "linuxgpio"
            << "-p" << partName
            << "-U" << (QString() 
                  + fuseName + ":w:0x" + QString::number(fuseValue, 16) + ":m")
    );
    if (!avrdude.waitForStarted()) {
        testError("Unable to start avrdude");
        return;
    }

    avrdude.closeWriteChannel();
    if (!avrdude.waitForFinished()) {
        testError("avrdude never finished");
        return;
    }

    if (avrdude.exitCode()) {
        testError(QString("avrdude returned an error: ") + avrdude.readAll());
        return;
    }
}
