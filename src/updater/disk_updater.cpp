#include "disk_updater.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>

#define UPDATER_FILENAME "cc-jig-updater.zip"

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

    foreach (QFileInfo mediaDeviceInfo, mediaDirectory.entryInfoList()) {
        if (mediaDeviceInfo.isDir()) {

            // Try opening the updater file.
            // We try to update it rather than testing it exists because it may
            // be that it's on a case-insensitive filesystem where the file
            // exists, but doesn't have the correct case.
            QFile updateFile(mediaDeviceInfo.absoluteFilePath() + QDir::separator() + UPDATER_FILENAME);
            if (! updateFile.open(QFile::ReadOnly)) {
                continue;
            }
            qDebug() << "Updating" << QFileInfo(updateFile).absoluteFilePath();
        }
    }
}
