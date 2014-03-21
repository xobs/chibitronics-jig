#include <QDebug>
#include "chibisequence.h"
#include "setpower.h"
#include "setvoltage.h"
#include "delay.h"
#include "header.h"
#include "programsticker.h"
#include "teststicker.h"
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
    _effectsTests.append(new Delay(100));
    _effectsTests.append(new SetPower(SetPower::powerOn));

    _effectsTests.append(new Header("Programming"));
    _effectsTests.append(new ProgramSticker("chibitrigger1.hex", 1));
    _effectsTests.append(new ProgramSticker("chibitrigger1.hex", 2));
    _effectsTests.append(new ProgramSticker("chibitrigger1.hex", 3));
    _effectsTests.append(new ProgramSticker("chibitrigger1.hex", 4));
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

    _effectsTests.append(new Header("Testing at 3.3V"));
    _effectsTests.append(new SetVoltage(SetVoltage::threeVolts));
    _effectsTests.append(new Delay(100));
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

    /* Wire up signals and slots for all tests */
    qDebug() << "Connecting signals...";
    for (int i = 0; i < _effectsTests.count(); i++)
        connect(
            _effectsTests.at(i),
            SIGNAL(testMessage(const QString,int,int,const QString)),
            this,
            SLOT(receiveTestMessage(const QString,int,int,const QString)));
    qDebug() << "Done";
}

const QList<ChibiTest *> & ChibiSequence::effectsTests()
{
    return _effectsTests;
}

/* Returns true if there are more tests to run */
bool ChibiSequence::runEffectsTests() {
    currentTestNumber = -1;
    errorCount = 0;
    testsToRun.clear();
    testsToRun = _effectsTests;
    return runNextTest();
}

void ChibiSequence::receiveTestMessage(const QString name,
                                       int type,
                                       int value, const QString message)
{
    if (type == ChibiTest::infoMessage)
        qDebug() << name << "INFO:" << value << message;
    else if (type == ChibiTest::errorMessage) {
        errorCount++;
        qDebug() << name << "ERROR:" << value << message;
    }
    else if (type == ChibiTest::debugMessage)
        qDebug() << name << "DEBUG:" << value << message;
    else if (type == ChibiTest::setHeader)
        emit setHeader(message);
    else if (type == ChibiTest::setStickerNum)
        emit setStickerNum(value);
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


bool ChibiSequence::runNextTest(int continueOnErrors)
{
    if (errorCount && !continueOnErrors && !debugMode) {
        QString str;
        str.append(testsToRun.at(currentTestNumber)->testName());
        emit testsFinished();
        return false;
    }

    // Increment the test number, and return if we've run out of tests.
    currentTestNumber++;
    if (currentTestNumber >= testsToRun.count()) {
        //ui->setProgressBar(1);
        //ui->finishTests(errorCount?false:true);
        return false;
    }

    currentTest = testsToRun[currentTestNumber];

    currentThread = new ChibiTestEngineThread(currentTest);
    QObject::connect(currentThread, SIGNAL(finished()),
                     this, SLOT(cleanupCurrentTest()));
    currentThread->start();

    return true;
}
