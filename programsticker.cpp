#include <QString>
#include <QProcess>
#include "programsticker.h"

ProgramSticker::ProgramSticker(QString firmware, int num)
    : stickerNum(num), firmwareFile(firmware)
{
    name = "Program one sticker";
}

void ProgramSticker::runTest()
{
    QProcess avrdude;

    emit testMessage(testName(), infoMessage, stickerNum, 
            QString("Programming sticker ") + QString::number(stickerNum));

    selectSticker(stickerNum);

    avrdude.start("./avrdude", QStringList()
            << "-C" << "chibi-sensor.conf"
            << "-c" << "linuxgpio"
            << "-P" << "linuxgpio"
            << "-p" << "attiny5"
            << "-U" << "flash:w:chibi-pattern.hex"
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
