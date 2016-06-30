#include <unistd.h>
#include <QDebug>
#include "chibisequence.h"
#include "setpower.h"
#include "setvoltage.h"
#include "setmicrodrive.h"
#include "unexportgpio.h"
#include "setgpio.h"
#include "delay.h"
#include "header.h"
#include "programsticker.h"
#include "verifysticker.h"
#include "setstickerfuse.h"
#include "teststicker.h"
#include "testaudio.h"
#include "testled.h"
#include "finished.h"

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
    /* Effects Sequence:
       1)  Toggle power off
       2)  Wait 100ms
       2)  Set power to 5V
       3)  Wait 100ms
       4)  Toggle power on
       5)  Program sticker[1]
       6)  Program sticker[2]
       7)  Program sticker[3]
       8)  Program sticker[4]
       9)  Toggle power off
       10) Wait 100ms
       11) Toggle power on
       12) Test output of sticker[1]
       13) Test output of sticker[2]
       14) Test output of sticker[3]
       15) Test output of sticker[4]
       16) Toggle power off
       17) Set power to 3V
       18) Toggle power on
       19) Test output of sticker[1]
       20) Test output of sticker[2]
       21) Test output of sticker[3]
       22) Test output of sticker[4]
       23) Toggle power off
    */
    _effectsTests.append(new Header("Reset Jig"));
    _effectsTests.append(new SetPower(SetPower::powerOff));
    _effectsTests.append(new Delay(100));
    _effectsTests.append(new SetVoltage(SetVoltage::fiveVolts));
    _effectsTests.append(new UnexportGpio(ChibiTest::tpiSignalGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::tpiDatGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::spiResetGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::spiSckGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::spiMosiGpio));
    _effectsTests.append(new UnexportGpio(ChibiTest::spiMisoGpio));
    _effectsTests.append(new Delay(100));
    _effectsTests.append(new SetPower(SetPower::powerOn));

    _effectsTests.append(new Header("Programming"));
    _effectsTests.append(new ProgramSticker(1, "chibi-pattern.hex"));
    _effectsTests.append(new ProgramSticker(2, "chibi-pattern.hex"));
    _effectsTests.append(new ProgramSticker(3, "chibi-pattern.hex"));
    _effectsTests.append(new ProgramSticker(4, "chibi-pattern.hex"));
    _effectsTests.append(new SetPower(SetPower::powerOff));
    _effectsTests.append(new Delay(100));

    _effectsTests.append(new Header("Testing at 5V"));
    _effectsTests.append(new SetPower(SetPower::powerOn));
    _effectsTests.append(new TestSticker(TestSticker::twinkleSticker, 1));
    _effectsTests.append(new TestSticker(TestSticker::heartbeatSticker, 2));
    _effectsTests.append(new TestSticker(TestSticker::blinkSticker, 3));
    _effectsTests.append(new TestSticker(TestSticker::fadeSticker, 4));
    _effectsTests.append(new SetPower(SetPower::powerOff));
    _effectsTests.append(new Delay(100));

    _effectsTests.append(new Header("Test completed"));
    _effectsTests.append(new Finished());

    /* Sensor sequence:
       1)  Toggle power off
       2)  Wait 100ms
       3)  Set power to 5V
       4)  Wait 100ms
       5)  Toggle power on
       6)  Program microcontroller
       7)  Program trigger circuit
       8)  Measure light sensor A/D
       9)  Turn on LED
       10) Wait 100ms
       11) Measure light sensor A/D
       12) Measure sound output
       13) Turn buzzer on
       14) Wait 100ms
       15) Measure sound output
       16) Turn buzzer off
       17) Apply DC bias with D/A converter, confirm no pulse
       18) Excursion to + threshold -- confirm pulse
       19) Excursion to - threshold -- confirm pulse
       20) Ask operator to trigger button switch
       21) Program final microcontroller behavior
       22) Toggle power off
    */
    _sensorTests.append(new Header("Reset Jig"));
    _sensorTests.append(new SetPower(SetPower::powerOff));
    _sensorTests.append(new Delay(100));
    _sensorTests.append(new SetVoltage(SetVoltage::fiveVolts));

    /* Reset these so we don't hear the buzzer during programming */
    _sensorTests.append(new SetGpio(ChibiTest::buzzerGpio, 0));
    _sensorTests.append(new SetGpio(ChibiTest::ledGpio, 0));

    _sensorTests.append(new UnexportGpio(ChibiTest::tpiSignalGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::tpiDatGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiResetGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiSckGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiMosiGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiMisoGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiResetMicroGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiSckMicroGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiMosiMicroGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiMisoMicroGpio));
    _sensorTests.append(new SetMicroDrive(SetMicroDrive::program));
    _sensorTests.append(new Delay(100));
    _sensorTests.append(new SetPower(SetPower::powerOn));

    _sensorTests.append(new Header("Programming trigger"));
    _sensorTests.append(new ProgramSticker(8, "chibi-trigger.hex"));
    _sensorTests.append(new VerifySticker(8, "chibi-trigger.hex"));
    _sensorTests.append(new SetPower(SetPower::powerOff));
    _sensorTests.append(new Delay(500));

    _sensorTests.append(new Header("Testing at 5V"));
    _sensorTests.append(new SetVoltage(SetVoltage::fiveVolts));
    _sensorTests.append(new SetMicroDrive(SetMicroDrive::execute));

    _sensorTests.append(new SetPower(SetPower::powerOn));
    _sensorTests.append(new TestLed(300));
    _sensorTests.append(new TestAudio(3 * 1000));
    _sensorTests.append(new SetPower(SetPower::powerOff));
    _sensorTests.append(new Delay(100));

    _sensorTests.append(new Header("Testing at 3.3V"));
    _sensorTests.append(new SetVoltage(SetVoltage::threeVolts));
    _sensorTests.append(new Delay(100));
    _sensorTests.append(new SetPower(SetPower::powerOn));
    _sensorTests.append(new TestLed(50));
    _sensorTests.append(new TestAudio(3 * 1000));
    _sensorTests.append(new Delay(100));

    /* Program Micro last, so it doesn't "learn" anything during testing */
    _sensorTests.append(new SetPower(SetPower::powerOff));
    _sensorTests.append(new Delay(100));
    _sensorTests.append(new SetVoltage(SetVoltage::fiveVolts));

    /* Reset these so we don't hear the buzzer during programming */
    _sensorTests.append(new SetGpio(ChibiTest::buzzerGpio, 0));
    _sensorTests.append(new SetGpio(ChibiTest::ledGpio, 0));

    _sensorTests.append(new UnexportGpio(ChibiTest::tpiSignalGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::tpiDatGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiResetGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiSckGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiMosiGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiMisoGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiResetMicroGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiSckMicroGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiMosiMicroGpio));
    _sensorTests.append(new UnexportGpio(ChibiTest::spiMisoMicroGpio));
    _sensorTests.append(new SetMicroDrive(SetMicroDrive::program));
    _sensorTests.append(new Delay(100));
    _sensorTests.append(new SetPower(SetPower::powerOn));

    _sensorTests.append(new Header("Programming micro"));
    _sensorTests.append(new ProgramSticker(5, "stickers_byte_attiny85_memorize_optimized.cpp.hex",
                                            "chibi-micro.conf", "attiny85"));
    _sensorTests.append(new VerifySticker(5, "stickers_byte_attiny85_memorize_optimized.cpp.hex",
                                            "chibi-micro.conf", "attiny85"));
    /* Disable self-programming of flash */
    _sensorTests.append(new SetStickerFuse(5, "efuse", 0xFF,
                                           "chibi-micro.conf", "t85"));
    /* Set brown-out detect to 1.8V */
    _sensorTests.append(new SetStickerFuse(5, "hfuse", 0xDE,
                                           "chibi-micro.conf", "t85"));
    /* Set clock to 8MHz */
    _sensorTests.append(new SetStickerFuse(5, "lfuse", 0xE2,
                                           "chibi-micro.conf", "t85"));

    _sensorTests.append(new Header("Test completed"));
    _sensorTests.append(new Finished());

    /* Wire up signals and slots for all tests */
    for (int i = 0; i < _effectsTests.count(); i++)
        connect(
            _effectsTests.at(i),
            SIGNAL(testMessage(const QString,int,int,const QString)),
            this,
            SLOT(receiveTestMessage(const QString,int,int,const QString)));

    for (int i = 0; i < _sensorTests.count(); i++)
        connect(
            _sensorTests.at(i),
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
    if (type == ChibiTest::infoMessage) {
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
    else if (type == ChibiTest::errorMessage) {
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
    else if (type == ChibiTest::debugMessage) {
        QString txt;
        txt = "DEBUG [" + name + "]: " + QString::number(value) + message;
        qDebug() << txt;
    }
    else if (type == ChibiTest::setHeader)
        emit setHeader(message);
    else if (type == ChibiTest::setStickerNum)
        emit setStickerNum(value);
    else if (type == ChibiTest::testPass)
        emit appendPass();
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
