#include <QString>
#include <QProcess>
#include "programsticker.h"

ProgramSticker::ProgramSticker(int num, QString firmware,
                               QString config, QString part)
    : stickerNum(num), firmwareFile(firmware),
    configFile(config), partName(part)
{
    name = "Program sticker";
}

void ProgramSticker::runTest()
{
    QProcess avrdude;

    emit testMessage(testName(), infoMessage, stickerNum, 
            QString("Programming sticker ") + QString::number(stickerNum));

    selectSticker(stickerNum);

    avrdude.start("./avrdude", QStringList()
            << "-C" << configFile
            << "-c" << "linuxgpio"
            << "-P" << "linuxgpio"
            << "-p" << partName
            << "-U" << (QString("flash:w:") + firmwareFile)
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
        testError(QString("Failed to program"));
        return;
    }
}
