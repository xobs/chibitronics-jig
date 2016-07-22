#ifdef __arm__
#include "testmodule.h"
#include <wiringPi.h>
#include <stddef.h>

static const FrameworkCallbacksC *mod_callbacks;

static void sendStart(void) {
    mod_callbacks->send_message(NULL, StartTests, NULL, NULL);
}

static void StartButton__init(const FrameworkCallbacksC *callbacks) {
    mod_callbacks = callbacks;

    // Note: we've hacked wiringPi.c to work when not run as root.
    wiringPiSetupGpio();
    pinMode(26, INPUT);
    pullUpDnControl(26, PUD_UP);
    wiringPiISR(26, INT_EDGE_FALLING, sendStart);
}

struct test_module_c Q_DECL_EXPORT test_module = {
    TEST_MODULE_MAGIC_C,
    StartButton__init,
    "StartButton",
    "Monitor the \"Start\" button",
    NULL,
    NULL,
    NULL,
    NULL,
};
#endif
