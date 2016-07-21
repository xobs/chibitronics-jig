#include "disk_updater.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QString>
#include <QStringList>

#define UPDATER_FILENAME "cc-updater.jig"

#include <QDebug>

DiskUpdater::DiskUpdater() {
    ;
}

void DiskUpdater::doCheck() {
#ifdef WIN64
    QDir mediaDirectory("C:\\Users\\Sean\\Code\\chibitronics-jig");
#else
    QDir mediaDirectory("/media");
#endif

    // Only allow one updater at a time to run.
    if (updateProcess)
        return;

    foreach (QFileInfo mediaDeviceInfo, mediaDirectory.entryInfoList()) {
        if (mediaDeviceInfo.isDir()) {

            // Try opening the updater file.
            // We try to update it rather than testing it exists because it may
            // be that it's on a case-insensitive filesystem where the file
            // exists, but doesn't have the correct case.
            QString updateFileName = mediaDeviceInfo.absoluteFilePath() + QDir::separator() + UPDATER_FILENAME;
            QFile updateFile(updateFileName);
            if (! updateFile.open(QFile::ReadOnly)) {
                continue;
            }
            qDebug() << "Updating" << QFileInfo(updateFile).absoluteFilePath();
            updateProcess = new QProcess();
            QStringList arguments;
            arguments << QString("0x") + QString(TESTVER).toInt(NULL, 16);

            connect(updateProcess, SIGNAL(finished(int)),
                    this, SLOT(updaterTerminated(int)));
            updateProcess->start(updateFileName, arguments);
        }
    }
}

void DiskUpdater::updaterTerminated(int result) {
    (void)result;

    // If we got here, it's because the update finished without updating
    // anything.  Allow the update to re-run later on.
    delete updateProcess;
    updateProcess = NULL;
}
