#include <QApplication>
#include <QGraphicsObject>
#include "qtquick1applicationviewer.h"
#include "chibisequence.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ChibiSequence chibiSequence(&app);

    app.setOverrideCursor(Qt::BlankCursor);

    QtQuick1ApplicationViewer viewer;
    viewer.addImportPath(QLatin1String("modules"));
    viewer.setOrientation(QtQuick1ApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/chibitronics-jig/main.qml"));
    viewer.showExpanded();

    QObject *rootObject = viewer.rootObject();
    QObject::connect(rootObject, SIGNAL(startTest()),
                     &chibiSequence, SLOT(runEffectsTests()));
    QObject::connect(&chibiSequence, SIGNAL(testFinished()),
                     rootObject, SLOT(nextStep()));
    QObject::connect(&chibiSequence, SIGNAL(setHeader(const QString)),
                     rootObject, SLOT(setHeader(const QString)));
    QObject::connect(&chibiSequence, SIGNAL(setStickerNum(int)),
                     rootObject, SLOT(setSticker(int)));
    QObject::connect(&chibiSequence, SIGNAL(testsFinished()),
                     rootObject, SLOT(testsFinished()));
    QObject::connect(&chibiSequence, SIGNAL(appendLog(const QString)),
		     rootObject, SLOT(appendLog(const QString)));
    QObject::connect(&chibiSequence, SIGNAL(appendError(const QString)),
		     rootObject, SLOT(appendError(const QString)));
    return app.exec();
}
