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

    const QString & name() {
        return _name;
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

const QString & delay__instance_name_qt(TestInstance *instance) {
    Delay *d = (Delay *)instance;
    return d->name();
}

void delay__instance_run(TestInstance *instance) {

    Delay *d = (Delay *)instance;
    d->runTest();
}

struct test_module Q_DECL_EXPORT test_module = {
    /* .magic = */              TEST_MODULE_MAGIC,
    /* .module_init = */        delay__init,
    /* .module_name = */        "Delay",
    /* .module_description = */ "Delay for a number of ms",
    /* .instance_init = */      delay__instance_init,
    /* .instance_name = */      NULL,
    /* .instance_name_qt = */   delay__instance_name_qt,
    /* .instance_run = */       delay__instance_run,
    /* .instance_free = */      NULL,
};
