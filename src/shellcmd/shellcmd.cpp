#include "testmodule.h"
#include <QString>
#include <QStringList>
#include <QProcess>

static const FrameworkCallbacksQt *mod_callbacks;
enum powerState {
    powerOn,
    powerOff,
};

class ShellCmd
{
    public:

        ShellCmd(void *new_key,
                 const QString & new_cmd,
                 const QStringList & new_args,
                 uint32_t timeout_msecs,
                 const QString & success_msg)
            : key(new_key),
              cmd(new_cmd),
              args(new_args),
              timeout(timeout_msecs),
              success_str(success_msg),
              _name(QString("Run command: %1").arg(cmd))
        {
        };

        void runTest(void)
        {
            /*
            if (state == powerOn) {
                mod_callbacks->test_message(key, infoMessageType, 0, "Turning power on");
                mod_callbacks->set_gpio(key, powerGpio, 1);
            }
            else {
                mod_callbacks->test_message(key, infoMessageType, 0, "Turning power off");
                mod_callbacks->set_gpio(key, powerGpio, 0);
            }
            */
        };

        const QString & name() {
            return _name;
        };

    private:
        void *key;
        QString cmd;
        QStringList args;
        uint32_t timeout;
        QString success_str;
        const QString _name;
};

static void shellcmd__init(const FrameworkCallbacksQt *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *shellcmd__instance_init(void *testObj,
                                                   QMap<QString, QVariant> params) {
    ShellCmd *sc = new ShellCmd(testObj,
                                params["command"].toString(),
                                params["args"].toStringList(),
                                params["timeout"].toUInt(),
                                params["success"].toString());

    return (TestInstance *)sc;
}

static const QString &shellcmd__instance_name(TestInstance *instance) {
    ShellCmd *sc = (ShellCmd *)instance;
    return sc->name();
}

static void shellcmd__instance_run(TestInstance *instance) {

    ShellCmd *sc = (ShellCmd *)instance;
    sc->runTest();
}

static void shellcmd__instance_free(TestInstance *instance) {

    ShellCmd *sc = (ShellCmd *)instance;
    delete sc;
}

extern "C" {
    struct test_module_qt Q_DECL_EXPORT test_module = {
        TEST_MODULE_MAGIC_QT,
        shellcmd__init,
        QString("ShellCmd"),
        QString("Run a shell command"),
        shellcmd__instance_init,
        shellcmd__instance_name,
        shellcmd__instance_run,
        shellcmd__instance_free,
    };
};
