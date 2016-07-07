#include "chibitest.h"
#include "chibitestregistry.h"

#include <QtGlobal>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QLibrary>

static void ct_test_message(void *testObj, TestMessageType messageType,
                            int value, const char *message) {
    static_cast<ChibiTest*>(testObj)->testData(
        static_cast<ChibiTest*>(testObj)->testName(),
        messageType,
        value,
        message);
}

static void ct_test_message_qt(void *testObj, TestMessageType messageType,
                               int value, const QString & message) {
    static_cast<ChibiTest*>(testObj)->testData(
        static_cast<ChibiTest*>(testObj)->testName(),
        messageType,
        value,
        message);
}

static void ct_msleep(void *testObj, int msecs) {
    static_cast<ChibiTest*>(testObj)->msleep(msecs);
}

static void ct_set_gpio(void *testObj, int gpio, int val) {
    static_cast<ChibiTest*>(testObj)->setGpio(gpio, val);
}

int  ct_get_gpio(void *testObj, int gpio) {
    return static_cast<ChibiTest*>(testObj)->getGpio(gpio);
}

void ct_unexport_gpio(void *testObj, int gpio) {
    static_cast<ChibiTest*>(testObj)->unexportGpio(gpio);
}

static const FrameworkCallbacks frameworkCallbacks = {
  /* test_message */    ct_test_message,
  /* msleep */          ct_msleep,
  /* set_gpio */        ct_set_gpio,
  /* get_gpio */        ct_get_gpio,
  /* unexport_gpio */   ct_unexport_gpio,
};

static const FrameworkCallbacksQt frameworkCallbacksQt = {
  /* test_message_qt */ ct_test_message_qt,
  /* msleep */          ct_msleep,
  /* set_gpio */        ct_set_gpio,
  /* get_gpio */        ct_get_gpio,
  /* unexport_gpio */   ct_unexport_gpio,
};


ChibiTestRegistry::ChibiTestRegistry() {
    addDirectory(QDir(QCoreApplication::applicationDirPath()));
}

bool ChibiTestRegistry::addDirectory(const QDir & dir) {

    foreach (QFileInfo item, dir.entryInfoList()) {

        if (!QLibrary::isLibrary(item.absoluteFilePath()))
            continue;

        QLibrary plugin(item.absoluteFilePath());

        QFunctionPointer module_ptr = plugin.resolve("test_module");
        if (!module_ptr)
            continue;

        if (!addModule((const void *)module_ptr))
            plugin.unload();
    }
    return true;
}

bool ChibiTestRegistry::addModule(const void *module) {

    const TestModuleC *module_c = (const TestModuleC *)module;
    const TestModuleQt *module_qt = (const TestModuleQt *)module;

    if (module_c->magic == TEST_MODULE_MAGIC) {
        /* Don't re-add duplicate modules */
        if (registry.value(module_c->module_name).value<void *>() != NULL)
            return false;
        module_c->module_init(&frameworkCallbacks);

        registry.insert(module_c->module_name,
                        QVariant::fromValue((void *)module_c));
    }

    else if (module_qt->magic == TEST_MODULE_MAGIC_QT) {
        /* Don't re-add duplicate modules */
        if (registry.value(module_qt->module_name).value<void *>() != NULL)
            return false;
        module_qt->module_init(&frameworkCallbacksQt);

        registry.insert(module_qt->module_name,
                        QVariant::fromValue((void *)module_qt));
    }
    return true;
}

const TestModule *ChibiTestRegistry::getModule(const QString &name) {
    void *module = registry.value(name).value<void *>();
    if (!module) {
        qWarning(QString("Unable to locate the module named \"%1\"").arg(name).toUtf8());
        return (const TestModule *)NULL;
    }
    return (const TestModule *)module;
}

const QStringList ChibiTestRegistry::moduleNames() {

    return registry.keys();
}

