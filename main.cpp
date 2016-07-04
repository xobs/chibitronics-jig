//#include <QApplication>
//#include <QGraphicsObject>
//#include "qtquick1applicationviewer.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "chibisequence.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    //QApplication app(argc, argv);
    QGuiApplication app(argc, argv);
    ChibiSequence chibiSequence(&app);

    //app.setOverrideCursor(Qt::BlankCursor);


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    /*
    viewer.addImportPath(QLatin1String("modules"));
    viewer.setOrientation(QtQuick1ApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/chibitronics-jig/main.qml"));
    viewer.showExpanded();
    */

    QObject *rootObject = engine.rootObjects().first();
    QObject *stickersTest = rootObject->findChild<QObject *>("stickersTest");

    QObject::connect(stickersTest, SIGNAL(startEffectsTests()),
                     &chibiSequence, SLOT(runEffectsTests()));
    QObject::connect(stickersTest, SIGNAL(startSensorTests()),
                     &chibiSequence, SLOT(runSensorTests()));

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
