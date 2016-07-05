#include "testmodule.h"
#include <QString>

static const FrameworkCallbacksQt *mod_callbacks;
enum powerState {
    powerOn,
    powerOff,
};

class ProgramSticker
{
    public:

        ProgramSticker(void *new_key, int new_gpio, enum powerState new_state)
            : key(new_key),
              state(new_state),
              powerGpio(new_gpio),
              _name(QString("Program sticker with %1").arg("thingy"))
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

        const QString & name() {
            return _name;
        };

    private:
        void *key;
        enum powerState state;
        int powerGpio;
        const QString _name;
};

static void programsticker__init(const FrameworkCallbacksQt *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *programsticker__instance_init(void *testObj,
                                                   QMap<QString, QVariant> params) {

    enum powerState state = powerOn; 
    int gpio = 0;

    /*
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
    */

    ProgramSticker *ps = new ProgramSticker(testObj, gpio, state);

    return (TestInstance *)ps;
}

static const QString &programsticker__instance_name(TestInstance *instance) {
    ProgramSticker *ps = (ProgramSticker *)instance;
    return ps->name();
}

static void programsticker__instance_run(TestInstance *instance) {

    ProgramSticker *ps = (ProgramSticker *)instance;
    ps->runTest();
}

static void programsticker__instance_free(TestInstance *instance) {

    ProgramSticker *ps = (ProgramSticker *)instance;
    delete ps;
}

struct test_module_qt Q_DECL_EXPORT test_module = {
    TEST_MODULE_MAGIC_QT,
    programsticker__init,
    QString("ProgramSticker"),
    QString("Program a sticker"),
    programsticker__instance_init,
    programsticker__instance_name,
    programsticker__instance_run,
    programsticker__instance_free,
};
