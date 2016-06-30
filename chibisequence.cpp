#include <unistd.h>
#include <QDebug>
#include <QThread>
#include "chibisequence.h"
#include "testmodule.h"

extern struct test_module set_power;
extern struct test_module delay;
extern struct test_module header;

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

ChibiSequence::ChibiSequence(QObject *parent) :
    QObject(parent)
{

    testRegistry.addModule(&set_power);
    testRegistry.addModule(&delay);
    testRegistry.addModule(&header);
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
//    _effectsTests.append(new ChibiTest(testRegistry.getModule("SetPower"),
//                "state", "on", NULL));
    _effectsTests.append(new ChibiTest(testRegistry.getModule("Header"),
                "message", "Step 1",
                NULL));
    _effectsTests.append(new ChibiTest(testRegistry.getModule("Delay"),
                "msecs", "1000",
                NULL));
    _effectsTests.append(new ChibiTest(testRegistry.getModule("Header"),
                "message", "Second Step",
                NULL));
    _effectsTests.append(new ChibiTest(testRegistry.getModule("Delay"),
                "msecs", "1000",
                NULL));
    _effectsTests.append(new ChibiTest(testRegistry.getModule("Header"),
                "message", "Number three",
                NULL));
    _effectsTests.append(new ChibiTest(testRegistry.getModule("Delay"),
                "msecs", "1000",
                NULL));
    _effectsTests.append(new ChibiTest(testRegistry.getModule("Header"),
                "message", "Finnish",
                NULL));
#if 0
    {
        QHash<QString, QVariant> testConfig;
        testConfig.insert("message", "Reset Jig");
        _effectsTests.append(new Header(testConfig));
    }
    {
        QHash<QString, QVariant> testConfig;
        testConfig.insert("power", "on");
        _effectsTests.append(new SetPower(testConfig));
    }
    _effectsTests.append(new Delay(100));
    _effectsTests.append(new SetVoltage(SetVoltage::fiveVolts));
    _effectsTests.append(new UnexportGpio(ChibiTest::tpiSignalGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::tpiDatGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::spiResetGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::spiSckGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::spiMosiGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::spiMisoGpio));
    _effectsTests.append(new Delay(100));

    {
        QHash<QString, QVariant> testConfig;
        testConfig.insert("message", "Programming");
        _effectsTests.append(new Header(testConfig));
    }
    _effectsTests.append(new ProgramSticker(1, "chibi-pattern.hex"));
    _effectsTests.append(new ProgramSticker(2, "chibi-pattern.hex"));
    _effectsTests.append(new ProgramSticker(3, "chibi-pattern.hex"));
    _effectsTests.append(new ProgramSticker(4, "chibi-pattern.hex"));
    _effectsTests.append(new Delay(100));

    {
        QHash<QString, QVariant> testConfig;
        testConfig.insert("message", "Testing at 5V");
        _effectsTests.append(new Header(testConfig));
    }
    _effectsTests.append(new TestSticker(TestSticker::twinkleSticker, 1));
    _effectsTests.append(new TestSticker(TestSticker::heartbeatSticker, 2));
    _effectsTests.append(new TestSticker(TestSticker::blinkSticker, 3));
    _effectsTests.append(new TestSticker(TestSticker::fadeSticker, 4));
    {
        QHash<QString, QVariant> testConfig;
        testConfig.insert("power", "off");
        _effectsTests.append(new SetPower(testConfig));
    }
    _effectsTests.append(new Delay(100));

    {
        QHash<QString, QVariant> testConfig;
        testConfig.insert("message", "Test Completed");
        _effectsTests.append(new Header(testConfig));
    }
    _effectsTests.append(new Finished());
#endif

    /* Wire up signals and slots for all tests */
    for (int i = 0; i < _effectsTests.count(); i++)
        connect(
            _effectsTests.at(i),
            SIGNAL(testMessage(const QString,int,int,const QString)),
            this,
            SLOT(receiveTestMessage(const QString,int,int,const QString)));

    log.setFileName("/home/aqs/stickers.log");
    if (!log.open(QFile::ReadWrite | QFile::Append)) {
        emit appendError("Unable to open logfile");
        qDebug() << "Unable to open logfile: " << log.errorString();
    }
}

const QList<ChibiTest *> & ChibiSequence::effectsTests()
{
    return _effectsTests;
}

/* Returns true if there are more tests to run */
bool ChibiSequence::runEffectsTests()
{
    currentTestNumber = -1;
    errorCount = 0;
    testsToRun.clear();
    testsToRun = _effectsTests;

    QString txt("---\n");
    QByteArray txtBytes = txt.toUtf8();
    // For some reason, log.write() doesn't work, and never calls write()
    write(log.handle(), txtBytes, txtBytes.size());

    return runNextTest();
}

bool ChibiSequence::runSensorTests()
{
    currentTestNumber = -1;
    errorCount = 0;
    testsToRun.clear();
    testsToRun = _sensorTests;

    QString txt("---\n");
    QByteArray txtBytes = txt.toUtf8();
    // For some reason, log.write() doesn't work, and never calls write()
    write(log.handle(), txtBytes, txtBytes.size());

    return runNextTest();
}

void ChibiSequence::receiveTestMessage(const QString name,
                                       int type,
                                       int value, const QString message)
{
    if (type == infoMessageType) {
        QString txt;
        QByteArray txtBytes;

        txt = "INFO [" + name + "]: " + QString::number(value) + " " + message;
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

        txt = "ERROR [" + name + "]: " + QString::number(value) + " " + message;
        qDebug() << txt;
        emit appendLog(txt);
        emit appendError(message);

        txt += "\n";
        txtBytes = txt.toUtf8();
        // For some reason, log.write() doesn't work, and never calls write()
        write(log.handle(), txtBytes, txtBytes.size());
    }
    else if (type == debugMessageType) {
        QString txt;
        txt = "DEBUG [" + name + "]: " + QString::number(value) + message;
        qDebug() << txt;
    }
    else if (type == testPassType)
        emit appendPass();
    else if (type == setHeaderType)
        emit setHeader(message);
    else
        qDebug() << name << "????:" << type << value << message;
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
