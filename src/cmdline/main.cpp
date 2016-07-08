#include <QCoreApplication>
#include <QVariant>
#include <QThread>
#include <QDebug>

#include "chibitest.h"
#include "chibitestregistry.h"

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
            qInfo() << txt;
        }
        else if (type == errorMessageType) {
            errorCount++;
            QString txt;
            QByteArray txtBytes;

            txt = "ERROR [" + name + "]: " + QString::number(value) + " " + message.toString();
            qInfo() << txt;
        }
        else if (type == debugMessageType) {
            QString txt;
            txt = "DEBUG [" + name + "]: " + QString::number(value) + message.toString();
            qInfo() << txt;
        }
        else if (type == testPassType)
            qInfo() << "Test passed";
        else if (type == setHeaderType)
            qInfo() << "Setting header to:" << message.toString();
        else
            qInfo() << name << "????:" << type << value << message.toString();
    };

	void testFinished(void)
	{
		qInfo() << "Test finished with" << errorCount << "errors";
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

    args.takeFirst();

    if (args.isEmpty()) {
        qInfo("Error: must specify a test name");
        qInfo("Available modules:");
        foreach(const QString moduleName, testRegistry.moduleNames()) {
            qInfo(QString("    %1").arg(moduleName).toUtf8());
        }
        qFatal("No test specified");
    }

    QString testName = args.takeFirst();
    qInfo() << "Test name:" << testName;

    while (!args.isEmpty()) {
        QString key = args.takeFirst();
        if (args.isEmpty())
            break;
        QString val = args.takeFirst();

        params.insert(key, val);
    }

    const TestModule *module = testRegistry.getModule(testName);

    if (!module) {
        qInfo() << "Available modules:";
        foreach (const QString name, testRegistry.moduleNames()) {
            qInfo() << name;
        }
        qFatal(QString("Unable to locate module").toUtf8());
    }

    ChibiTest *test = new ChibiTest(module, params);
    qInfo() << "Running test:" << test->testName();
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
