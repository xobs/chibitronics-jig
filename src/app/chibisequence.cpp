#include <QtGlobal>
#include <QThread>

#include <QDebug>
#include "chibisequence.h"
#include "testmodule.h"
#include "chibilogger.h"

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

ChibiSequence::ChibiSequence(QObject *parent,
                             const QVariant & tests,
                             const QString & _logPath,
                             const QString & logfile) :
    QObject(parent),
    logPath(_logPath),
    isRunning(false)
{
    connect(
        &testRegistry,
        SIGNAL(testMessage(const QString,int,const QVariant,const QVariant)),
        this,
        SLOT(dispatchMessage(const QString,int,const QVariant,const QVariant)));

    /* For each test present, resolve the test to a plugin module and wire it up. */
    foreach (const QVariant & var, tests.toList()) {
        const QMap<QString, QVariant> plugin = var.toMap();
        const TestModule *module = testRegistry.getModule(plugin["testName"].toString());

        if (!module)
            qFatal(QString("Unable to locate module").toUtf8());

        ChibiTest *test = new ChibiTest(module,
                                        plugin["params"].toMap(),
                                        plugin["testTitle"].toString());
        test->setRegistry(&testRegistry);
        connect(
            test,
            SIGNAL(testMessage(const QString,int,const QVariant,const QVariant)),
            this,
            SLOT(dispatchMessage(const QString,int,const QVariant,const QVariant)));
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

    logger = new ChibiLogger(logPath + QDir::separator() + "chibilog.db");
}

void ChibiSequence::testEngineLoaded()
{
    /* Report to the UI a list of tests */
    QStringList testNames;
    foreach (ChibiTest *test, _tests)
        testNames.append(test->testName());
    emit testListUpdated(testNames);
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
    testRegistry.resetVariables();
    advanceTest = true;

    QString txt("---\n");
    QByteArray txtBytes = txt.toUtf8();
    log.write(txtBytes);

    emit testsStarted();
    return runNextTest();
}

void ChibiSequence::dispatchMessage(const QString & name,
                                    int type,
                                    const QVariant & message,
                                    const QVariant & parameter)
{
    if (type == InfoMessage) {
        QString txt;
        QByteArray txtBytes;

        txt = message.toString();
        qDebug() << txt;
        emit appendLog(txt);

        txt += "\n";
        txtBytes = txt.toUtf8();
        log.write(txtBytes);
    }
    else if ((type == ErrorMessage) || (type == FatalMessage)) {
        errorCount++;
        QString txt;
        QByteArray txtBytes;

        txt = "ERROR [" + name + "]: " + message.toString();
        qDebug() << txt;
        emit appendLog(txt);
        emit appendError(message.toString());

        txt += "\n";
        txtBytes = txt.toUtf8();
        log.write(txtBytes);

        // If it's fatal, don't allow us to advance the test.
        if (type == FatalMessage)
            advanceTest = false;
    }
    else if (type == DebugMessage) {
        QString txt;
        txt = "DEBUG [" + name + "]: " + message.toString();
        qDebug() << txt;
    }
    else if (type == TestPass)
        emit appendPass();
    else if (type == SetHeader)
        emit setHeader(message.toString());
    else if (type == StartTests)
        runTests();
    else if (type == TestingPoint)
        emit addPoint(message, 0);
    else if (type == PassPoint)
        emit addPoint(message, 1);
    else if (type == FailPoint)
        emit addPoint(message, 2);
    else if (type == SetVariable) {
        testRegistry.setVariable(message, parameter);
        emit setVariable(message, parameter);
    }
    else if (type == SetGlobal) {
        testRegistry.setGlobal(message, parameter);
        emit setGlobal(message, parameter);
    }
    else
        qDebug() << name << "????:" << type << message.toString();
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
    if (!advanceTest || currentTestNumber >= testsToRun.count()) {
        isRunning = false;
        emit testsFinished();
        return false;
    }

    currentTest = testsToRun[currentTestNumber];
    qDebug() << "Starting up next test" << currentTest->testName();
    if (currentTest->testHeader() != "")
        emit setHeader(currentTest->testHeader());

    currentThread = new ChibiTestEngineThread(currentTest);
    QObject::connect(currentThread, SIGNAL(finished()),
                     this, SLOT(cleanupCurrentTest()));
    currentThread->start();

    return true;
}
