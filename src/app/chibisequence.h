#ifndef CHIBISEQUENCE_H
#define CHIBISEQUENCE_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QVariant>
#include "chibitest.h"
#include "chibitestregistry.h"

class ChibiTestEngineThread;
class ChibiLogger;

class ChibiSequence : public QObject
{
	Q_OBJECT
public:
    explicit ChibiSequence(QObject *parent, const QString & logPath = QString(), const QString & logFileName = QString());

private:
    QList<ChibiTest *> _tests;
    QList<ChibiTest *> testsToRun;
    ChibiTestRegistry testRegistry;

    ChibiTest *currentTest;
    ChibiLogger *logger;
    int currentTestNumber;
    ChibiTestEngineThread *currentThread;
    QFile log;
    QString logPath;
    bool isRunning;
    int errorCount;
    bool debugMode;
    bool advanceTest;

    bool runSelectedTests(QList<ChibiTest *> &tests);

    bool runNextTest();

public slots:
    /* 
     @param level - 0 == info, 1 == error, 2 == debug
     @param value - An error code.  0 for success.
     @param message - An informative message to put up.
    */
    void dispatchMessage(const QString & name,
                         int testMessageType,
                         const QVariant & message,
                         const QVariant & param);
    void cleanupCurrentTest(void);
    bool runTests(void);

    /* Called after the test engine is loaded, but just before it is displayed */
    void testEngineLoaded(void);

    /* This sets the current test list to the JSON object specified */
    void setTests(QVariant);

signals:
    void testFinished();
    void testsFinished();
    void testsStarted();
    void addPoint(const QVariant & pt, const QVariant & state);
    void setHeader(const QVariant & header);
    void appendLog(const QVariant & message);
    void appendError(const QVariant & message);
    void appendPass();
    void testListUpdated(const QVariant & testNames);

    /* Called when individual variables are set */
    void setVariable(const QVariant & key, const QVariant & param);
    void setGlobal(const QVariant & key, const QVariant & param);
};

#endif // CHIBISEQUENCE_H
