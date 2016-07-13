#include <QtGlobal>
#include <QThread>

#include <QDebug>
#include "chibisequence.h"
#include "testmodule.h"

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

ChibiSequence::ChibiSequence(QObject *parent, const QVariant & tests, const QString & logfile) :
    QObject(parent),
    isRunning(false)
{
    /* For each test present, resolve the test to a plugun module and wire it up. */
    foreach (const QVariant & var, tests.toList()) {
        const QMap<QString, QVariant> plugin = var.toMap();
        const TestModule *module = testRegistry.getModule(plugin["testName"].toString());

        if (!module) {
            qFatal(QString("Unable to locate module").toUtf8());
        }

        ChibiTest *test = new ChibiTest(module,
                                        plugin["params"].toMap());
        connect(
            test,
            SIGNAL(testMessage(const QString,int,int,const QVariant)),
            this,
            SLOT(receiveTestMessage(const QString,int,int,const QVariant)));
        _tests.append(test);
    }

    /* Open the resulting logfile */
    if (logfile != "") {
        log.setFileName(logfile);
        if (!log.open(QFile::ReadWrite | QFile::Append)) {
            emit appendError("Unable to open logfile");
            qDebug() << "Unable to open logfile: " << log.errorString();
        }
    }
}

/* Returns true if there are more tests to run */
bool ChibiSequence::runTests()
{
    if (isRunning)
        return true;
    isRunning = true;
    currentTestNumber = -1;
    errorCount = 0;
    testsToRun.clear();
    testsToRun = _tests;

    QString txt("---\n");
    QByteArray txtBytes = txt.toUtf8();
    log.write(txtBytes);

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
        log.write(txtBytes);
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
        log.write(txtBytes);
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
    else if (type == startTestsType)
        runTests();
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
        isRunning = false;
        emit testsFinished();
        return false;
    }

    currentTest = testsToRun[currentTestNumber];
    qDebug() << "Starting up next test" << currentTest->testName();

    currentThread = new ChibiTestEngineThread(currentTest);
    QObject::connect(currentThread, SIGNAL(finished()),
                     this, SLOT(cleanupCurrentTest()));
    currentThread->start();

    return true;
}
