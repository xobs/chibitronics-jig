#include "testmodule.h"
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDir>

#define INTERESTING_STRING_PREFIX "!>>)} "

static const FrameworkCallbacksQt *mod_callbacks;

class TakePicture
{
    public:

        TakePicture(void *new_key,
                 const QString & path,
                 uint32_t timeout_msecs)
            : key(new_key),
              outputPath(path),
              timeout(timeout_msecs),
              _name(QString(QObject::tr("Take a selfie")))
        {
            cmd = "raspistill";
            if (!timeout)
                timeout = 10000; // The picture can take a while
        };

        void runTest(void)
        {
            QProcess process;
            QStringList args;

            QDir::root().mkpath(outputPath);
            args << "-n"
                 << "-r"
                 << "-o" << QString("%1%2%3.jpg").arg(outputPath).arg(QDir::separator()).arg(mod_callbacks->get_variable(key, "serial").toString());

            // Combine stdout and stderr into one stream.
            process.setProcessChannelMode(QProcess::MergedChannels);

            process.start(cmd, args);

            // Wait for the process to end.
            process.waitForFinished(timeout);

            // If it's still running, that's a problem.
            if (process.state() != QProcess::NotRunning) {
                mod_callbacks->send_message(key, ErrorMessage, QString(QObject::tr("Process timed out")), NULL);
                process.terminate();
                return;
            }

            QByteArray output = process.readAll();

            // Log all interesting lines to the test framework.
            // Interesting lines start with INTERESTING_STRING_PREFIX
            foreach (QString line, output.split('\n')) {
                if (line.startsWith(INTERESTING_STRING_PREFIX)) {
                    mod_callbacks->send_message(key, InfoMessage, line.remove(0, sizeof(INTERESTING_STRING_PREFIX)), NULL);
                }
            }

            // Make sure the process didn't crash.
            if (process.exitStatus() != QProcess::NormalExit) {
                mod_callbacks->send_message(key, InfoMessage, QString(QObject::tr("Process did not have a normal exit")), NULL);
                mod_callbacks->send_message(key, DebugMessage, output, NULL);
                return;
            }

            // Make sure the exit code is good.
            if (process.exitCode() != 0) {
                mod_callbacks->send_message(key, InfoMessage, QString(QObject::tr("Process exited with code %1")).arg(process.exitCode()), NULL);
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
        QString outputPath;
        uint32_t timeout;
        const QString _name;
};

static void TakePicture__init(const FrameworkCallbacksQt *callbacks) {
    mod_callbacks = callbacks;
}

static TestInstance *TakePicture__instance_init(void *testObj,
                                                   QMap<QString, QVariant> params) {
    TakePicture *sc = new TakePicture(testObj,
                                params["path"].toString(),
                                params["timeout"].toUInt());
    return (TestInstance *)sc;
}

static const QString &TakePicture__instance_name(TestInstance *instance) {
    TakePicture *sc = (TakePicture *)instance;
    return sc->name();
}

static void TakePicture__instance_run(TestInstance *instance) {

    TakePicture *sc = (TakePicture *)instance;
    sc->runTest();
}

static void TakePicture__instance_free(TestInstance *instance) {

    TakePicture *sc = (TakePicture *)instance;
    delete sc;
}

extern "C" {
    struct test_module_qt Q_DECL_EXPORT test_module = {
        TEST_MODULE_MAGIC_QT,
        TakePicture__init,
        QString("TakePicture"),
        QString("Take a picture of the board"),
        TakePicture__instance_init,
        TakePicture__instance_name,
        TakePicture__instance_run,
        TakePicture__instance_free,
    };
};
