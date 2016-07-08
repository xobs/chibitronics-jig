//#include <QApplication>
//#include <QGraphicsObject>
//#include "qtquick1applicationviewer.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QVariant>
#include "chibisequence.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    //QApplication app(argc, argv);
    QGuiApplication app(argc, argv);

    //app.setOverrideCursor(Qt::BlankCursor);


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //engine.load(QUrl(QStringLiteral("src/app/main.qml")));

    /*
    viewer.addImportPath(QLatin1String("modules"));
    viewer.setOrientation(QtQuick1ApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/chibitronics-jig/main.qml"));
    viewer.showExpanded();
    */

    QObject *rootObject = engine.rootObjects().first();
    QVariant tests = QQmlProperty::read(rootObject, "tests");
    QObject *stickersTest = rootObject->findChild<QObject *>("stickersTest");

    ChibiSequence chibiSequence(&app, tests);

    QObject::connect(stickersTest, SIGNAL(startTests()),
                     &chibiSequence, SLOT(runTests()));

    QObject::connect(&chibiSequence, SIGNAL(testFinished()),
                     stickersTest, SLOT(onNextStep()));
    QObject::connect(&chibiSequence, SIGNAL(setHeader(const QVariant)),
                     stickersTest, SLOT(onSetHeader(const QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(setStickerNum(const QVariant)),
                     stickersTest, SLOT(onSetSticker(const QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(testsFinished()),
                     stickersTest, SLOT(onTestsFinished()));
    QObject::connect(&chibiSequence, SIGNAL(appendLog(const QVariant)),
		     stickersTest, SLOT(onAppendLog(const QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(appendError(const QVariant)),
		     stickersTest, SLOT(onAppendError(const QVariant)));
    QObject::connect(&chibiSequence, SIGNAL(appendPass()),
		     stickersTest, SLOT(onAppendPass()));

    return app.exec();
}
