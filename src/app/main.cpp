#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QVariant>
#include "chibisequence.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOverrideCursor(Qt::BlankCursor);


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *rootObject = engine.rootObjects().first();
    QVariant tests = QQmlProperty::read(rootObject, "tests");
    QVariant logPath = QQmlProperty::read(rootObject, "logPath");

    QObject *stickersTest = rootObject->findChild<QObject *>("stickersTest");

    ChibiSequence chibiSequence(&app, logPath.toString());

    QObject::connect(stickersTest, SIGNAL(startTests()),
                     &chibiSequence, SLOT(runTests()));
    QObject::connect(stickersTest,SIGNAL(sendMessage(QString,int,QVariant,QVariant)),
                     &chibiSequence, SLOT(dispatchMessage(QString,int,QVariant,QVariant)));
    QObject::connect(stickersTest, SIGNAL(setTests(QVariant)),
                     &chibiSequence, SLOT(setTests(QVariant)));
    QObject::connect(stickersTest, SIGNAL(quit()),
                     &app, SLOT(quit()));

    QObject::connect(&chibiSequence, SIGNAL(testFinished()),
                     stickersTest, SLOT(onNextStep()));
    QObject::connect(&chibiSequence, SIGNAL(setHeader(const QVariant)),
                     stickersTest, SLOT(onSetHeader(const QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(testsFinished()),
                     stickersTest, SLOT(onTestsFinished()));
    QObject::connect(&chibiSequence, SIGNAL(appendLog(const QVariant)),
                    stickersTest, SLOT(onAppendLog(const QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(appendError(const QVariant)),
                    stickersTest, SLOT(onAppendError(const QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(appendPass()),
                    stickersTest, SLOT(onAppendPass()));
    QObject::connect(&chibiSequence, SIGNAL(addPoint(const QVariant, const QVariant)),
                     stickersTest, SLOT(addPoint(const QVariant, const QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(testsStarted()),
                     stickersTest, SLOT(testsStarted()));
    QObject::connect(&chibiSequence, SIGNAL(testFinished()),
                     stickersTest, SLOT(testFinished()));
    QObject::connect(&chibiSequence, SIGNAL(testListUpdated(const QVariant)),
                     stickersTest, SLOT(onTestListUpdated(const QVariant)));

    QObject::connect(&chibiSequence, SIGNAL(setVariable(QVariant,QVariant)),
                     stickersTest, SLOT(onSetVariable(QVariant,QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(setGlobal(QVariant,QVariant)),
                     stickersTest, SLOT(onSetGlobal(QVariant,QVariant)));

    chibiSequence.setTests(tests);
    chibiSequence.testEngineLoaded();
    return app.exec();
}
