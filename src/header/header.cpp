#include <QString>
#include "testmodule.h"

static const FrameworkCallbacksQt *mod_callbacks;

class Header {

private:
    void *obj;
    QString str;
    QString _name;

public:
    Header(void *new_obj, const QString &new_str) :
        obj(new_obj),
        str(new_str),
        _name(QString("Header with message: \"%1\"").arg(new_str))
    {
    };

    void runTest()
    {
        mod_callbacks->test_message(obj, setHeaderType, 0, str);
    };

    const QString & name()
    {
        return _name;
    }
};

static void header__init(const FrameworkCallbacksQt *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *header__instance_init(void *testObj,
                                           QMap<QString, QVariant> params) {

    Header *h = new Header(testObj, params["message"].toString());

    return (TestInstance *)h;
}

const QString &header__instance_name(TestInstance *instance) {

    Header *h = (Header *)instance;
    return h->name();
}

void header__instance_run(TestInstance *instance) {

    Header *h = (Header *)instance;
    h->runTest();
}

extern "C" struct test_module_qt Q_DECL_EXPORT test_module = {
    TEST_MODULE_MAGIC_QT,
    header__init,
    QString("Header"),
    QString("Display a header"),
    header__instance_init,
    header__instance_name,
    header__instance_run,
    NULL,
};
