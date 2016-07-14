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
    module(new_module)
{
    va_list ap;

    va_start(ap, new_module);
    instance = module->c.instance_init(this, ap);
    va_end(ap);
}

ChibiTest::ChibiTest(const TestModule *new_module,
                     const QMap<QString, QVariant> & params,
                     const QString & new_header) :
    module(new_module),
    header(new_header)
{
    instance = module->qt.instance_init(this, params);
}

const QString ChibiTest::testName()
{
    if (module->c.magic == TEST_MODULE_MAGIC_C) {
        if (module->c.instance_name)
            return QString(module->c.instance_name(instance));
        else
            return QString(module->c.module_name);
    }
    else if (module->qt.magic == TEST_MODULE_MAGIC_QT) {
        if (module->qt.instance_name)
            return module->qt.instance_name(instance);
        else
            return module->qt.module_name;
    }
    else
        return QString();
}

const QString ChibiTest::testHeader()
{
    return header;
}

void ChibiTest::runTest()
{
    if (module->c.magic == TEST_MODULE_MAGIC_C)
        module->c.instance_run(instance);
    else if (module->qt.magic == TEST_MODULE_MAGIC_QT)
        module->qt.instance_run(instance);
}

void ChibiTest::testInfo(const QString &string)
{
    emit testMessage(testName(), InfoMessage, string);
}

void ChibiTest::testError(const QString &string)
{
    emit testMessage(testName(), ErrorMessage, string);
}

void ChibiTest::testDebug(const QString &string)
{
    emit testMessage(testName(), DebugMessage, string);
}

void ChibiTest::testData(const QString & name, int testMessageType,
                         const QVariant & message) {
    emit testMessage(name, testMessageType, message);
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
