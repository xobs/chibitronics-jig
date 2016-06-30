#ifndef CHIBITEST_H
#define CHIBITEST_H

#include <QString>
#include <QObject>
#include <QHash>
#include <QVariant>

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
    const QHash<QString, QVariant> _settings;

protected:
    QString name;
    void setGpio(int gpio, int val);
    int getGpio(int gpio);
    int unexportGpio(int gpio);
    void selectSticker(int stickerNum);
    void msleep(int msec);

public:
    ChibiTest(const QHash<QString, QVariant> &settings = QHash<QString, QVariant>());
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
        testPass,
    };

    enum gpio {
        powerGpio = 17,
        outputGpio = 54,
        microDriveGpio = 57,
        voltageGpio = 59,

        tpiSignalGpio = 67,
        tpiDatGpio = 68,

        /* SPI lines for effects stickers */
        spiResetGpio = 64,
        spiSckGpio = 66,
        spiMosiGpio = 65,
        spiMisoGpio = 53,

        /* Separate SPI lines for Microcontroller sticker */
        spiResetMicroGpio = 69,
        spiSckMicroGpio = 70,
        spiMosiMicroGpio = 72,
        spiMisoMicroGpio = 52,

        /* Sensor GPIOs */
        buzzerGpio = 76,
        ledGpio = 75,
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
