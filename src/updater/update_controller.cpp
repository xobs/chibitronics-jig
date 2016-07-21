#include "update_controller.h"
#include "network_updater.h"
#include "disk_updater.h"

UpdateController::UpdateController() {
    networkUpdater = new NetworkUpdater();
    diskUpdater = new DiskUpdater();

    // The update process can take a while, so we move the check process to its
    // own thread.  That way, the process can take as long as it likes.
    diskUpdater->moveToThread(&checkThread);
    networkUpdater->moveToThread(&checkThread);

    connect(&networkCheckTimer, SIGNAL(timeout()),
            networkUpdater, SLOT(doCheck()));
    connect(&diskCheckTimer, SIGNAL(timeout()),
            diskUpdater, SLOT(doCheck()));
    checkThread.start();
}

UpdateController::~UpdateController() {
    checkThread.quit();
    checkThread.wait();
}

void UpdateController::startChecking(unsigned int networkCheckTime, unsigned int diskCheckTime) {

    networkCheckTimer.setSingleShot(false);
    diskCheckTimer.setSingleShot(false);

    networkCheckTimer.start(networkCheckTime * 1000);
    diskCheckTimer.start(diskCheckTime * 1000);
}
