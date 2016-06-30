#include <stdarg.h>
#include <QString>
#include "testmodule.h"

static const FrameworkCallbacks *mod_callbacks;

class Delay {

private:
    unsigned int msecs;
    void *obj;
    const QString _name;

public:
    Delay(void *new_obj, int new_msecs) :
        msecs(new_msecs),
        obj(new_obj),
        _name(QString("Delay for %1 msecs").arg(msecs))
    {
    };

    void runTest() {
        mod_callbacks->msleep(obj, msecs);
    };

    const char *name() {
        return _name.toUtf8();
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

const char *delay__instance_name(TestInstance *instance) {
    Delay *d = (Delay *)instance;
    return d->name();
}

void delay__instance_run(TestInstance *instance) {

    Delay *d = (Delay *)instance;
    d->runTest();
}

struct test_module delay = {
    delay__init,
    "Delay",
    "Delay for a number of ms",
    delay__instance_init,
    delay__instance_name,
    delay__instance_run,
    NULL,
    NULL,
    NULL,
};
