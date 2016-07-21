#include "disk_updater.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDevice>
#include <QProcess>
#include <QString>
#include <QStringList>

#define UPDATER_FILENAME "cc-updater.jig"
#define TEMP_FILE "/tester/updater"
#define MOUNT_DIR "/media"

DiskUpdater::DiskUpdater() :
    updateProcess(NULL)
{
    ;
}

void DiskUpdater::doCheck() {
    QDir mediaDirectory(MOUNT_DIR);

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
            if (! updateFile.open(QFile::ReadOnly))
                continue;

            QFile outputFile(TEMP_FILE);
            if (!outputFile.open(QFile::WriteOnly))
                continue;

            while (!updateFile.atEnd()) {
                QByteArray bytes = updateFile.read(4096);
                outputFile.write(bytes);
            }
            updateFile.close();
            outputFile.close();
            outputFile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner
                                    | QFileDevice::ReadUser  | QFileDevice::WriteUser  | QFileDevice::ExeUser
                                    | QFileDevice::ReadGroup | QFileDevice::ExeGroup
                                    | QFileDevice::ReadOther | QFileDevice::ExeOther);

            updateProcess = new QProcess();
            QStringList arguments;
            arguments << QString("0x") + QString::number(TESTVER, 16);

            connect(updateProcess, SIGNAL(finished(int)),
                    this, SLOT(updaterTerminated(int)));
            updateProcess->start(TEMP_FILE, arguments);
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
