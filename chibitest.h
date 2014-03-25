#ifndef CHIBITEST_H
#define CHIBITEST_H

#include <QString>
#include <QObject>

class ChibiTestEngine;

#define TEST_ERROR 1
#define TEST_INFO 0
#define TEST_DEBUG 2


class ChibiTest : public QObject
{
    Q_OBJECT

private:
    int testNumber;
    QString lastString;
    int lastResult;
    ChibiTestEngine *engine;

protected:
    QString name;
    void setGpio(int gpio, int val);
    int getGpio(int gpio);
    void selectSticker(int stickerNum);
    void msleep(int msec);

public:
    ChibiTest();
    virtual void runTest() = 0;

    /* Called by the engine after a test has finished running */
    const QString &getStatusString();
    int getStatusValue();

    /* Used for issuing callbacks */
    void setEngine(ChibiTestEngine *engie);

    /* Tells this test what its position is */
    void setTestNumber(int number);
    const QString testName();

    enum testMessageType {
        infoMessage,
        errorMessage,
        debugMessage,
        setHeader,
        setStickerNum,
    };

public slots:
    void testInfo(const QString string);
    void testError(const QString string);
    void testDebug(const QString string);

signals:
    void testMessage(const QString name, int testMessageType,
                     int value, const QString message);
};

#endif // CHIBITEST_H
