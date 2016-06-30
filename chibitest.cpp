#include "chibitest.h"
#include <QProcess>
#include <QThread>

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

ChibiTest::ChibiTest(const QHash<QString, QVariant> &settings)
    : lastString(""), _settings(settings)
{
    testNumber = -1;
    lastResult = 0;
    engine = NULL;
}

const QString &ChibiTest::getStatusString()
{
    return lastString;
}

int ChibiTest::getStatusValue()
{
    return lastResult;
}

void ChibiTest::setTestNumber(int number)
{
    testNumber = number;
}

void ChibiTest::setEngine(ChibiTestEngine *engie)
{
    engine = engie;
}

const QString ChibiTest::testName()
{
    return name;
}

void ChibiTest::testInfo(const QString string)
{
    emit testMessage(testName(), infoMessage, 0, string);
}

void ChibiTest::testError(const QString string)
{
    emit testMessage(testName(), errorMessage, 0, string);
}

void ChibiTest::testDebug(const QString string)
{
    emit testMessage(testName(), debugMessage, 0, string);
}

void ChibiTest::setGpio(int gpio, int val)
{
    QProcess setGpio;
    setGpio.start("./gpio_out", QStringList() << QString::number(gpio) << QString::number(val));
    if (!setGpio.waitForStarted()) {
        testError("Unable to start gpio_out process");
        return;
    }

    setGpio.closeWriteChannel();

    if (!setGpio.waitForFinished()) {
        testError("gpio_out never finished");
        return;
    }

    if (setGpio.exitCode()) {
        testError(QString("gpio_out failed: ") + setGpio.readAll());
        return;
    }
}

int ChibiTest::getGpio(int gpio)
{
    QProcess getGpio;
    getGpio.start("./gpio_in", QStringList() << QString::number(gpio));
    if (!getGpio.waitForStarted()) {
        testError("Unable to start gpio_in process");
        return -1;
    }

    getGpio.closeWriteChannel();

    if (!getGpio.waitForFinished()) {
        testError("gpio_in never finished");
        return -1;
    }

    if (getGpio.exitCode() != 1 && getGpio.exitCode() != 0) {
        testError(QString("gpio_in failed: ") + getGpio.readAll());
        return -1;
    }
    return getGpio.exitCode();
}

int ChibiTest::unexportGpio(int gpio)
{
    QProcess unexport;
    unexport.start("./gpio_unexport", QStringList() << QString::number(gpio));
    if (!unexport.waitForStarted()) {
        testError("Unable to start gpio_unexport process");
        return -1;
    }

    unexport.closeWriteChannel();

    if (!unexport.waitForFinished()) {
        testError("gpio_unexport never finished");
        return -1;
    }

    if (unexport.exitCode())
        testError(QString("gpio_unexport failed: ") + unexport.readAll());
    return unexport.exitCode();
}

void ChibiTest::selectSticker(int stickerNum)
{
    QProcess s;

    emit testMessage(testName(), setStickerNum, stickerNum, "");

    if (stickerNum > 4)
        return;

    s.start("./select_sticker", QStringList() << QString::number(stickerNum));

    if (!s.waitForStarted()) {
        testError("Unable to select sticker");
        return;
    }

    s.closeWriteChannel();
    s.waitForFinished();
    if (s.exitCode()) {
        testError(QString("select_sticker returned an error: ") + s.readAll());
        return;
    }
}

void ChibiTest::msleep(int msecs)
{
    SleeperThread::msleep(msecs);
}
