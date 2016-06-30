#include <QString>
#include "testmodule.h"

static const FrameworkCallbacks *mod_callbacks;

class Header {

private:
    void *obj;
    QString str;
    QString _name;

public:
    Header(void *new_obj, const char *new_str) :
        obj(new_obj),
        str(new_str),
        _name(QString("Header with message: \"%1\"").arg(new_str))
    {
    };

    void runTest()
    {
        mod_callbacks->test_message_qt(obj, setHeaderType, 0, &str);
    };

    const char *name()
    {
        return _name.toUtf8();
    }
};

static void header__init(const FrameworkCallbacks *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *header__instance_init(void *testObj, va_list ap) {

    const char *key;
    const char *val;
    const char *str = NULL;

    while (1) {
        key = va_arg(ap, const char *);
        if (!key)
            break;
        val = va_arg(ap, const char *);
        if (!val)
            break;

        QString keyStr(key);
        QString valStr(val);

        if (keyStr == "message") {
            str = val;
        }
    }

    Header *h = new Header(testObj, str);

    return (TestInstance *)h;
}

const char *header__instance_name(TestInstance *instance) {

    Header *h = (Header *)instance;
    return h->name();
}

void header__instance_run(TestInstance *instance) {

    Header *h = (Header *)instance;
    h->runTest();
}

struct test_module header = {
    TEST_MODULE_MAGIC,
    header__init,
    "Header",
    "Display a header",
    header__instance_init,
    header__instance_name,
    header__instance_run,
    NULL,
};
