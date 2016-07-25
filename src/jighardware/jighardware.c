#ifdef __arm__
#include "testmodule.h"
#include <wiringPi.h>
#include <stddef.h>

#define START_BUTTON_GPIO 26
#define MODE_SELECT_GPIO 16

static const FrameworkCallbacksC *mod_callbacks;

static void sendStart(void) {
    mod_callbacks->send_message(NULL, StartTests, NULL, NULL);
}

static void JigHardware__init(const FrameworkCallbacksC *callbacks) {
    mod_callbacks = callbacks;

    // Note: we've hacked wiringPi.c to work when not run as root.
    wiringPiSetupGpio();

    // Set the start button up as a GPIO input
    pinMode(START_BUTTON_GPIO, INPUT);
    pullUpDnControl(START_BUTTON_GPIO, PUD_UP);
    wiringPiISR(START_BUTTON_GPIO, INT_EDGE_FALLING, sendStart);

    // We use GPIO 16 (SPI CE2, header pin 36) to differentiate between the
    // LtC and Dataviewer jigs.
    // The Dataviewer pulls this to GND, while the LtC board leaves it floating.
    pinMode(MODE_SELECT_GPIO, INPUT);
    pullUpDnControl(MODE_SELECT_GPIO, PUD_UP);

    if (! digitalRead(MODE_SELECT_GPIO)) {
        mod_callbacks->send_message(NULL, SetGlobal, "devicetype", "dataviewer");
    }
    else {
        mod_callbacks->send_message(NULL, SetGlobal, "devicetype", "ltc");
    }
}

struct test_module_c Q_DECL_EXPORT test_module = {
    TEST_MODULE_MAGIC_C,
    JigHardware__init,
    "JigHardware",
    "Monitor the \"Start\" button",
    NULL,
    NULL,
    NULL,
    NULL,
};
#endif
