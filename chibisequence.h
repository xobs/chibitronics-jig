#ifndef CHIBISEQUENCE_H
#define CHIBISEQUENCE_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QVariant>
#include "chibitest.h"
#include "chibitestregistry.h"

class ChibiTestEngineThread;

class ChibiSequence : public QObject
{
	Q_OBJECT
public:
	explicit ChibiSequence(QObject *parent = 0);

private:
    QList<ChibiTest *> _effectsTests;
    QList<ChibiTest *> _sensorTests;
    QList<ChibiTest *> testsToRun;
    ChibiTestRegistry testRegistry;

    ChibiTest *currentTest;
    int currentTestNumber;
    ChibiTestEngineThread *currentThread;
    QFile log;
    int errorCount;
    bool debugMode;

    bool runSelectedTests(QList<ChibiTest *> &tests);
    const QList<ChibiTest *> & effectsTests();

    bool runNextTest();

public slots:
    /* 
     @param level - 0 == info, 1 == error, 2 == debug
     @param value - An error code.  0 for success.
     @param message - An informative message to put up.
    */
    void receiveTestMessage(const QString name,
                            int testMessageType,
                            int value,
                            const QVariant message);
    void cleanupCurrentTest(void);
    bool runEffectsTests(void);
    bool runSensorTests(void);

signals:
    void testFinished();
    void testsFinished();
    void setHeader(const QVariant header);
    void setStickerNum(const QVariant stickerNum);
    void appendLog(const QVariant message);
    void appendError(const QVariant message);
    void appendPass();
};

#endif // CHIBISEQUENCE_H
