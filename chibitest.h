#ifndef CHIBITEST_H
#define CHIBITEST_H

#include <QString>
#include <QObject>
#include <QHash>
#include <QVariant>
#include "testmodule.h"

class ChibiTestEngine;

#define TEST_ERROR 1
#define TEST_INFO 0
#define TEST_DEBUG 2

class ChibiTest : public QObject
{
    Q_OBJECT

private:
    const TestModule *module;
    TestInstance *instance;
    QString name;

public:
    ChibiTest(const TestModule *module, ...);
    void runTest();

    const QString testName();

    /* Called by the engine after a test has finished running */
    /*
    const QString &getStatusString();
    int getStatusValue();
    */

/*protected:  // these methods are called from the C wrapper */
    void setGpio(int gpio, int val);
    int getGpio(int gpio);
    int unexportGpio(int gpio);
    void msleep(int msec);

public slots:
    void testInfo(const QString &string);
    void testError(const QString &string);
    void testDebug(const QString &string);
    void testData(const QString name, int testMessageType,
                  int value, const QString &message);

signals:
    void testMessage(const QString name, int testMessageType,
                     int value, const QString message);
};

#endif // CHIBITEST_H
