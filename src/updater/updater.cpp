#include "testmodule.h"
#include "update_controller.h"

#define NETWORK_CHECK_TIME (60*10)
#define DISK_CHECK_TIME (2)

static const FrameworkCallbacksQt *mod_callbacks;


static void delay__init(const FrameworkCallbacksQt *callbacks) {
    mod_callbacks = callbacks;

    UpdateController *updateController = new UpdateController();
    updateController->startChecking(NETWORK_CHECK_TIME, DISK_CHECK_TIME);
}

extern "C" struct test_module_qt Q_DECL_EXPORT test_module = {
    /* .magic = */              TEST_MODULE_MAGIC_QT,
    /* .module_init = */        delay__init,
    /* .module_name = */        QString("Updater"),
    /* .module_description = */ QString("Support auto-update of the tester"),
    /* .instance_init = */      NULL,
    /* .instance_name = */      NULL,
    /* .instance_run = */       NULL,
    /* .instance_free = */      NULL,
};
