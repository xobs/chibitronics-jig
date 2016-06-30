#include <stdarg.h>
#include <QString>
#include "testmodule.h"

static const FrameworkCallbacks *mod_callbacks;

class Delay {

private:
    unsigned int msecs;
    void *obj;

public:
    Delay(void *new_obj, int new_msecs) :
        msecs(new_msecs),
        obj(new_obj)
    {
    };

    void runTest() {
        mod_callbacks->msleep(obj, msecs);
    };
};

static void delay__init(const FrameworkCallbacks *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *delay__instance_init(void *testObj, va_list ap) {

    unsigned int msecs = 0;
    const char *key;
    const char *val;

    while (1) {
        key = va_arg(ap, const char *);
        if (!key)
            break;
        val = va_arg(ap, const char *);
        if (!val)
            break;

        QString keyStr(key);
        QString valStr(val);

        if (keyStr == "msecs") {
            msecs = valStr.toInt();
        }
    }

    Delay *d = new Delay(testObj, msecs);

    return (TestInstance *)d;
}

void delay__instance_run(TestInstance *instance) {

    Delay *d = (Delay *)instance;
    d->runTest();
}

struct test_module delay = {
    "Delay",
    "Delay for a number of ms",
    delay__init,
    delay__instance_init,
    delay__instance_run,
    NULL,
    NULL,
    NULL,
};
