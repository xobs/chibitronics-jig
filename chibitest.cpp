#include <stdarg.h>
#include <QProcess>
#include <QThread>
#include "chibitest.h"
#include "testmodule.h"

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

ChibiTest::ChibiTest(const TestModule *new_module, ...) :
    module(new_module),
    name(new_module->name)
{
    va_list ap;

    va_start(ap, new_module);
    instance = module->instance_init(this, ap);
    va_end(ap);
}

const QString ChibiTest::testName()
{
    return name;
}

void ChibiTest::runTest()
{
    module->instance_run(instance);
}

void ChibiTest::testInfo(const QString &string)
{
    emit testMessage(testName(), infoMessageType, 0, string);
}

void ChibiTest::testError(const QString &string)
{
    emit testMessage(testName(), errorMessageType, 0, string);
}

void ChibiTest::testDebug(const QString &string)
{
    emit testMessage(testName(), debugMessageType, 0, string);
}

void ChibiTest::testData(const QString name, int testMessageType,
                         int value, const QString &message) {
    emit testMessage(name, testMessageType, value, message);
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

void ChibiTest::msleep(int msecs)
{
    SleeperThread::msleep(msecs);
}
