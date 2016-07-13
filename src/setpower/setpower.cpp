#include "testmodule.h"
#include <QString>

static const FrameworkCallbacksQt *mod_callbacks;
enum powerState {
    powerOn,
    powerOff,
};

class SetPower
{
    public:

        SetPower(void *new_key, int new_gpio, enum powerState new_state)
            : key(new_key),
              state(new_state),
              powerGpio(new_gpio)
        {
        };

        void runTest(void)
        {
            if (state == powerOn) {
                mod_callbacks->send_message(key, InfoMessage, "Turning power on");
                mod_callbacks->send_message(key, SetGpio, powerGpio);
            }
            else {
                mod_callbacks->send_message(key, InfoMessage, "Turning power off");
                mod_callbacks->send_message(key, ClearGpio, powerGpio);
            }
        };

        const char * getStatusStr() {
            return "Ok";
        };

        int getStatus() {
            return 0;
        };

    private:
        void *key;
        enum powerState state;
        int powerGpio;
};

static void set_power_init(const FrameworkCallbacksQt *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *set_power_instance_init(void *testObj,
                                             QMap<QString, QVariant> params) {

    SetPower *sp = new SetPower(testObj, params.value("gpio").value<uint32_t>(), params.value("state") == "on" ? powerOn : powerOff);

    return (TestInstance *)sp;
}

void set_power_instance_run(TestInstance *instance) {

    SetPower *sp = (SetPower *)instance;
    sp->runTest();
}

void set_power_instance_free(TestInstance *instance) {

    SetPower *sp = (SetPower *)instance;
    delete sp;
}

extern "C" struct test_module_qt Q_DECL_EXPORT test_module = {
    TEST_MODULE_MAGIC_QT,
    set_power_init,
    QString("SetPower"),
    QString("Set stickers power"),
    set_power_instance_init,
    NULL,
    set_power_instance_run,
    set_power_instance_free,
};
