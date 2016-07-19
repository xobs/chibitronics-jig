#include "testmodule.h"
#include <QString>
#include <QStringList>
#include <QProcess>

#define INTERESTING_STRING_PREFIX "!>>)} "
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
              _name(QString("Run command: %1  Looking for: \"%2\"  Waiting up to %3 msecs").arg(cmd).arg(success_str).arg(timeout))
        {
        };

        void runTest(void)
        {
            QProcess process;
            process.setProgram(cmd);
            process.setArguments(args);

            // Combine stdout and stderr into one stream.
            process.setProcessChannelMode(QProcess::MergedChannels);

            process.start();

            // Wait for the process to end.
            process.waitForFinished(timeout);

            // If it's still running, that's a problem.
            if (process.state() != QProcess::NotRunning) {
                mod_callbacks->send_message(key, ErrorMessage, QString("Process timed out"));
                process.terminate();
                return;
            }

            QByteArray output = process.readAll();

            // Log all interesting lines to the test framework.
            // Interesting lines start with INTERESTING_STRING_PREFIX
            foreach (QString line, output.split('\n')) {
                if (line.startsWith(INTERESTING_STRING_PREFIX)) {
                    mod_callbacks->send_message(key, InfoMessage, line.remove(0, sizeof(INTERESTING_STRING_PREFIX)));
                }
            }

            // Make sure the process didn't crash.
            if (process.exitStatus() != QProcess::NormalExit) {
                mod_callbacks->send_message(key, ErrorMessage, QString("Process did not have a normal exit"));
                mod_callbacks->send_message(key, DebugMessage, output);
                return;
            }

            // Make sure the exit code is good.
            if (process.exitCode() != 0) {
                mod_callbacks->send_message(key, ErrorMessage, QString("Process exited with code %1").arg(process.exitCode()));
                mod_callbacks->send_message(key, DebugMessage, output);
                return;
            }

            // Look for our search string.
            if (success_str.length() && !output.contains(success_str.toUtf8())) {
                mod_callbacks->send_message(key, ErrorMessage, QString("Unable to find search string"));
                mod_callbacks->send_message(key, DebugMessage, output);
                return;
            }

            mod_callbacks->send_message(key, TestPass, "");
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
