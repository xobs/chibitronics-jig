#include <QCoreApplication>
#include <QVariant>
#include <QThread>

#include "chibitest.h"
#include "chibitestregistry.h"

#include <QDebug>

class TestRunner : public QObject
{
    Q_OBJECT

private:
	int errorCount;

public:
    TestRunner() : errorCount(0) {}

public slots:
    void receiveTestMessage(const QString name,
                            int type,
                            int value,
                            const QVariant message)
    {
        if (type == infoMessageType) {
            QString txt;
            QByteArray txtBytes;

            txt = "INFO [" + name + "]: " + QString::number(value) + " " + message.toString();
            qDebug() << txt;
        }
        else if (type == errorMessageType) {
            errorCount++;
            QString txt;
            QByteArray txtBytes;

            txt = "ERROR [" + name + "]: " + QString::number(value) + " " + message.toString();
            qDebug() << txt;
        }
        else if (type == debugMessageType) {
            QString txt;
            txt = "DEBUG [" + name + "]: " + QString::number(value) + message.toString();
            qDebug() << txt;
        }
        else if (type == testPassType)
            qDebug() << "Test passed";
        else if (type == setHeaderType)
            qDebug() << "Setting header to:" << message.toString();
        else
            qDebug() << name << "????:" << type << value << message.toString();
    };

	void testFinished(void)
	{
		qDebug() << "Test finished with" << errorCount << "errors";
	}
};

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



#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    ChibiTestRegistry testRegistry;
    QStringList args = app.arguments();
    QMap<QString, QVariant> params;
    TestRunner testRunner;
	ChibiTestEngineThread *currentThread;

    qDebug() << "Program name:" << args.takeFirst();

    if (args.isEmpty()) {
        qFatal("Error: must specify a test name");
    }

    QString testName = args.takeFirst();
    qDebug() << "Test name:" << testName;

    while (!args.isEmpty()) {
        QString key = args.takeFirst();
        qDebug() << "Key: " << key;
        if (args.isEmpty())
            break;
        QString val = args.takeFirst();
        qDebug() << "Val: " << val;

        params.insert(key, val);
    }

    const TestModule *module = testRegistry.getModule(testName);

    if (!module) {
        qDebug() << "Available modules:";
        foreach (const QString name, testRegistry.moduleNames()) {
            qDebug() << name;
        }
        qFatal(QString("Unable to locate module").toUtf8());
    }

    ChibiTest *test = new ChibiTest(module, params);
    qDebug() << "Running test:" << test->testName();
    QObject::connect(test,
                     SIGNAL(testMessage(const QString,int,int,const QVariant)),
                     &testRunner,
                     SLOT(receiveTestMessage(const QString,int,int,const QVariant)));

    currentThread = new ChibiTestEngineThread(test);
    QObject::connect(currentThread, SIGNAL(finished()),
                     &testRunner, SLOT(testFinished()));
    QObject::connect(currentThread, SIGNAL(finished()),
                     &app, SLOT(quit()));
    currentThread->start();

    return app.exec();
}
