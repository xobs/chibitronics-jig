#include <QString>
#include <QProcess>
#include "verifysticker.h"

VerifySticker::VerifySticker(int num, QString firmware,
                               QString config, QString part)
    : stickerNum(num), firmwareFile(firmware),
    configFile(config), partName(part)
{
    name = "Verify sticker";
}

void VerifySticker::runTest()
{
    QProcess avrdude;

    emit testMessage(testName(), infoMessage, stickerNum, 
            QString("Verifying sticker ") + QString::number(stickerNum));

    selectSticker(stickerNum);

    avrdude.start("./avrdude", QStringList()
            << "-C" << configFile
            << "-c" << "linuxgpio"
            << "-P" << "linuxgpio"
            << "-p" << partName
            << "-U" << (QString("flash:v:") + firmwareFile)
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
        testError(QString("Failed to verify"));
        return;
    }
    emit testMessage(testName(), testPass, stickerNum, "Test passed");
}
