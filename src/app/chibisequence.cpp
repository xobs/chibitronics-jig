#include <unistd.h>
#include <QtGlobal>
#include <QThread>

#include <QDebug>
#include "chibisequence.h"
#include "testmodule.h"

/*
extern struct test_module set_power;
extern struct test_module delay;
extern struct test_module header;
*/

class ChibiTest;
class ChibiTestEngineThread : public QThread {
    ChibiTest *tst;
public:
    ChibiTestEngineThread(ChibiTest *t)
    {
        tst = t;
    }
    void run()
    {
        tst->runTest();
    };
    const QString testName()
    {
        return tst->testName();
    }
};

ChibiSequence::ChibiSequence(QObject *parent, const QVariant & tests) :
    QObject(parent)
{

    //testRegistry.addModule(&set_power);
    //testRegistry.addModule(&delay);
    //testRegistry.addModule(&header);
    /* LtC sticker:
       1)  Toggle power off
       2)  Wait 100ms
       3)  Toggle power on
       4)  Program LtC sticker
       5)  Prompt operator to press/hold "reset button"
       6)  Program test .ino sketch via audio port
       7)  Toggle A0
       8)  Toggle A1
       9)  Toggle A3
       10) Toggle D0
       11) Toggle D1
       12) Check for A2 being "High"
       13) Prompt operator to press/hold Reset button
       14) Load physical programming .ino file
       15) Check that Program Fail LED is not lit
       16) Toggle power to sticker
       17) Verify red LED is not on
       18) Toggle power off
    */
    foreach (const QVariant & var, tests.toList()) {
        const QMap<QString, QVariant> plugin = var.toMap();
        const TestModule *module = testRegistry.getModule(plugin["testName"].toString());

        if (!module) {
            qFatal(QString("Unable to locate module").toUtf8());
        }

        ChibiTest *test = new ChibiTest(module,
                                        plugin["params"].toMap());
        _tests.append(test);
    }

    /* Wire up signals and slots for all tests */
    for (int i = 0; i < _tests.count(); i++) {
        qDebug() << "Test " << i << ": " << _tests.at(i)->testName();
        connect(
            _tests.at(i),
            SIGNAL(testMessage(const QString,int,int,const QVariant)),
            this,
            SLOT(receiveTestMessage(const QString,int,int,const QVariant)));
    }

    log.setFileName("/home/aqs/stickers.log");
    if (!log.open(QFile::ReadWrite | QFile::Append)) {
        emit appendError("Unable to open logfile");
        qDebug() << "Unable to open logfile: " << log.errorString();
    }
}

/* Returns true if there are more tests to run */
bool ChibiSequence::runTests()
{
    currentTestNumber = -1;
    errorCount = 0;
    testsToRun.clear();
    testsToRun = _tests;

    QString txt("---\n");
    QByteArray txtBytes = txt.toUtf8();
    // For some reason, log.write() doesn't work, and never calls write()
    write(log.handle(), txtBytes, txtBytes.size());

    return runNextTest();
}

void ChibiSequence::receiveTestMessage(const QString name,
                                       int type,
                                       int value, const QVariant message)
{
    if (type == infoMessageType) {
        QString txt;
        QByteArray txtBytes;

        txt = "INFO [" + name + "]: " + QString::number(value) + " " + message.toString();
        qDebug() << txt;
        emit appendLog(txt);

        txt += "\n";
        txtBytes = txt.toUtf8();
        // For some reason, log.write() doesn't work, and never calls write()
        write(log.handle(), txtBytes, txtBytes.size());
    }
    else if (type == errorMessageType) {
        errorCount++;
        QString txt;
        QByteArray txtBytes;

        txt = "ERROR [" + name + "]: " + QString::number(value) + " " + message.toString();
        qDebug() << txt;
        emit appendLog(txt);
        emit appendError(message.toString());

        txt += "\n";
        txtBytes = txt.toUtf8();
        // For some reason, log.write() doesn't work, and never calls write()
        write(log.handle(), txtBytes, txtBytes.size());
    }
    else if (type == debugMessageType) {
        QString txt;
        txt = "DEBUG [" + name + "]: " + QString::number(value) + message.toString();
        qDebug() << txt;
    }
    else if (type == testPassType)
        emit appendPass();
    else if (type == setHeaderType)
        emit setHeader(message.toString());
    else
        qDebug() << name << "????:" << type << value << message.toString();
}

void ChibiSequence::cleanupCurrentTest()
{
    delete currentThread;
    currentThread = NULL;
    emit testFinished();
    runNextTest();
    return;
}

bool ChibiSequence::runNextTest()
{
    // Increment the test number, and return if we've run out of tests.
    currentTestNumber++;
    if (currentTestNumber >= testsToRun.count()) {
        emit testsFinished();
        return false;
    }

    currentTest = testsToRun[currentTestNumber];

    currentThread = new ChibiTestEngineThread(currentTest);
    QObject::connect(currentThread, SIGNAL(finished()),
                     this, SLOT(cleanupCurrentTest()));
    currentThread->start();

    return true;
}
