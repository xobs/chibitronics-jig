#include "testmodule.h"
#include <QString>
#include <QStringList>
#include <QProcess>

#define INTERESTING_STRING_PREFIX "!>>)} "
#define IDREG_PREFIX "!>>)}!! "
#define SUCCESS_STRING ")))>>-- Done Programming --<<((("

static const FrameworkCallbacksQt *mod_callbacks;

class SwdProgrammer
{
    public:

        SwdProgrammer(void *new_key,
                 const QString & elfname,
                 QStringList idregisters,
                 uint32_t timeout_msecs,
                 QStringList extras)
            : key(new_key),
              elfName(elfname),
              idRegisters(idregisters),
              timeout(timeout_msecs),
              _name(QString(QObject::tr("Program SWD with %1")).arg(elfname))
        {
            cmd = "openocd";
            args  << "-f" << "interface/raspberrypi2-native.cfg"
                    << "-c" << "transport select swd"
                    << "-f" << "target/klx.cfg"
                    << "-c" << "klx.cpu configure -rtos ChibiOS"
                    << "-c" << "reset_config srst_push_pull"
                    << "-c" << "init"
                    << "-c" << "reset halt";
            foreach (const QString & region, idRegisters) {
                args << "-c" << "echo -n \"" IDREG_PREFIX "\""
                       << "-c" << QString("mdw %1").arg(region);
            }
            args << "-c" << QString("flash write_image %1").arg(elfName);
            foreach (const QString & ex, extras) {
                args << "-c" << ex;
            }
            args << "-c" << "reset";
            args << "-c" << "echo \"" SUCCESS_STRING "\"";
            args << "-c" << "exit";
        };

        bool resetChip(void)
        {
            QProcess process;
            QStringList resetArgs;
            resetArgs  << "-f" << "interface/raspberrypi2-native.cfg"
                    << "-c" << "transport select swd"
                    << "-f" << "target/klx.cfg"
                    << "-c" << "klx.cpu configure -rtos ChibiOS"
                    << "-c" << "reset_config srst_push_pull"
                    << "-c" << "init"
                    << "-c" << "kinetis mdm mass_erase"
                    << "-c" << "reset halt"
                    << "-c" << "exit";

            process.setProgram(cmd);
            process.setArguments(resetArgs);
            process.setProcessChannelMode(QProcess::MergedChannels);
            process.start();
            process.waitForFinished(timeout);

            QByteArray output = process.readAll();

            if (process.state() != QProcess::NotRunning) {
                mod_callbacks->send_message(key, DebugMessage, QString(QObject::tr("Reset process timed out")), NULL);
                process.terminate();
                mod_callbacks->send_message(key, DebugMessage, output, NULL);
                return false;
            }

            // Make sure the process didn't crash.
            if (process.exitStatus() != QProcess::NormalExit) {
                mod_callbacks->send_message(key, DebugMessage, QString(QObject::tr("Reset process did not have a normal exit")), NULL);
                mod_callbacks->send_message(key, DebugMessage, output, NULL);
                return false;
            }

            // Make sure the exit code is good.
            if (process.exitCode() != 0) {
                mod_callbacks->send_message(key, DebugMessage, QString(QObject::tr("Reset process exited with code %1")).arg(process.exitCode()), NULL);
                mod_callbacks->send_message(key, DebugMessage, output, NULL);
                return false;
            }

            mod_callbacks->send_message(key, DebugMessage, "Reset CPU", NULL);
            return true;
        }

        void runTest(void)
        {
            QProcess process;

            int reset_tries;
            bool did_reset = false;
            for (reset_tries = 0; !did_reset && reset_tries < 5; reset_tries++) {
                if (resetChip())
                    did_reset = true;
            }
            if (!did_reset) {
                mod_callbacks->send_message(key, FatalMessage, QString(QObject::tr("Reset process did not complete")), NULL);
                return;
            }

            process.setProgram(cmd);
            process.setArguments(args);

            // Combine stdout and stderr into one stream.
            process.setProcessChannelMode(QProcess::MergedChannels);

            process.start();

            // Wait for the process to end.
            process.waitForFinished(timeout);

            // If it's still running, that's a problem.
            if (process.state() != QProcess::NotRunning) {
                mod_callbacks->send_message(key, FatalMessage, QString(QObject::tr("Process timed out")), NULL);
                process.terminate();
                mod_callbacks->send_message(key, DebugMessage, QString(process.readAll()), NULL);
                return;
            }

            QByteArray output = process.readAll();
            QString serialNumber;

            // Log all interesting lines to the test framework.
            // Interesting lines start with INTERESTING_STRING_PREFIX
            foreach (QString line, output.split('\n')) {
                if (line.startsWith(INTERESTING_STRING_PREFIX)) {
                    mod_callbacks->send_message(key, InfoMessage, line.remove(0, sizeof(INTERESTING_STRING_PREFIX)), NULL);
                }
                else if (line.startsWith(IDREG_PREFIX)) {
                    QStringList components = line.trimmed().split(" ");
                    if (serialNumber.length() != 0)
                        serialNumber += "-";
                    serialNumber += components.last();
                }
            }

            mod_callbacks->send_message(key, SetVariable, "serial", serialNumber);

            // Make sure the process didn't crash.
            if (process.exitStatus() != QProcess::NormalExit) {
                mod_callbacks->send_message(key, FatalMessage, QString(QObject::tr("Process did not have a normal exit")), NULL);
                mod_callbacks->send_message(key, DebugMessage, output, NULL);
                return;
            }

            // Make sure the exit code is good.
            if (process.exitCode() != 0) {
                mod_callbacks->send_message(key, FatalMessage, QString(QObject::tr("Process exited with code %1")).arg(process.exitCode()), NULL);
                mod_callbacks->send_message(key, DebugMessage, output, NULL);
                return;
            }

            // Look for our search string.
            if (!output.contains(SUCCESS_STRING)) {
                mod_callbacks->send_message(key, FatalMessage, QString(QObject::tr("Unable to find success string")), NULL);
                mod_callbacks->send_message(key, DebugMessage, output, NULL);
                return;
            }

            mod_callbacks->send_message(key, TestPass, NULL, NULL);
        };

        const QString & name() {
            return _name;
        };

    private:
        void *key;
        QString cmd;
        QStringList args;
        QString elfName;
        QStringList idRegisters;
        uint32_t timeout;
        const QString _name;
};

static void SwdProgrammer__init(const FrameworkCallbacksQt *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *SwdProgrammer__instance_init(void *testObj,
                                                   QMap<QString, QVariant> params) {
    SwdProgrammer *sc = new SwdProgrammer(testObj,
                                params["elfname"].toString(),
                                params["idregisters"].toStringList(),
                                params["timeout"].toUInt(),
                                params["extras"].toStringList());
    return (TestInstance *)sc;
}

static const QString &SwdProgrammer__instance_name(TestInstance *instance) {
    SwdProgrammer *sc = (SwdProgrammer *)instance;
    return sc->name();
}

static void SwdProgrammer__instance_run(TestInstance *instance) {

    SwdProgrammer *sc = (SwdProgrammer *)instance;
    sc->runTest();
}

static void SwdProgrammer__instance_free(TestInstance *instance) {

    SwdProgrammer *sc = (SwdProgrammer *)instance;
    delete sc;
}

extern "C" {
    struct test_module_qt Q_DECL_EXPORT test_module = {
        TEST_MODULE_MAGIC_QT,
        SwdProgrammer__init,
        QString("SwdProgram"),
        QString("Program via SWD"),
        SwdProgrammer__instance_init,
        SwdProgrammer__instance_name,
        SwdProgrammer__instance_run,
        SwdProgrammer__instance_free,
    };
};
