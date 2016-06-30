#include "testmodule.h"
#include <QString>

static const FrameworkCallbacks *mod_callbacks;
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
                mod_callbacks->test_message(key, infoMessageType, 0, "Turning power on");
                mod_callbacks->set_gpio(key, powerGpio, 1);
            }
            else {
                mod_callbacks->test_message(key, infoMessageType, 0, "Turning power off");
                mod_callbacks->set_gpio(key, powerGpio, 0);
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

static void set_power_init(const FrameworkCallbacks *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *set_power_instance_init(void *testObj, va_list ap) {

    const char *key;
    const char *val;
    enum powerState state = powerOn; 
    int gpio = 0;

    while (1) {
        key = va_arg(ap, const char *);
        if (!key)
            break;
        val = va_arg(ap, const char *);
        if (!val)
            break;

        QString keyStr(key);
        QString valStr(val);

        if (keyStr == "state") {
            if (valStr == "on")
                state = powerOn;
            else if (valStr == "off")
                state = powerOff;
        }
        else if (keyStr == "gpio") {
            gpio = valStr.toInt();
        }
    }

    SetPower *sp = new SetPower(testObj, gpio, state);

    return (TestInstance *)sp;
}

void set_power_instance_run(TestInstance *instance) {

    SetPower *sp = (SetPower *)instance;
    sp->runTest();
}

const char * set_power_instance_status_str(TestInstance *instance) {

    SetPower *sp = (SetPower *)instance;
    return sp->getStatusStr();
}

int set_power_instance_status(TestInstance *instance) {

    SetPower *sp = (SetPower *)instance;
    return sp->getStatus();
}

void set_power_instance_free(TestInstance *instance) {

    SetPower *sp = (SetPower *)instance;
    delete sp;
}

struct test_module set_power = {
  "SetPower",
  "Set stickers power",
  set_power_init,
  set_power_instance_init,
  set_power_instance_run,
  set_power_instance_status_str,
  set_power_instance_status,
  set_power_instance_free,
};
