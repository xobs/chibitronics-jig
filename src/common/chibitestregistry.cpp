#include "chibitest.h"
#include "chibitestregistry.h"

#include <QtGlobal>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QLibrary>

extern struct test_module_qt Q_DECL_IMPORT test_module;
static ChibiTestRegistry *currentTestRegistry;

static void ct_test_message_c(void *testObj, TestMessageType messageType,
                            const void *message, const void *param) {
    if (testObj)
        static_cast<ChibiTest*>(testObj)->testData(
            static_cast<ChibiTest*>(testObj)->testName(),
            messageType,
            message,
            param);
    else
        emit currentTestRegistry->testMessage(QString(),
                                              messageType,
                                              message,
                                              param);
}

static const void * ct_get_variable_c(void *testObj, const void *key) {
    if (testObj)
        return static_cast<ChibiTest*>(testObj)->getRegistry()->getVariable(key).value<void *>();
    else
        return currentTestRegistry->getVariable(key).value<void *>();
}

static const void * ct_get_global_c(void *testObj, const void *key) {
    if (testObj)
        return static_cast<ChibiTest*>(testObj)->getRegistry()->getGlobal(key).value<void *>();
    else
        return currentTestRegistry->getGlobal(key).value<void *>();
}

static void ct_test_message_qt(void *testObj, TestMessageType messageType,
                               const QVariant & message, const QVariant & param) {
    if (testObj)
        static_cast<ChibiTest*>(testObj)->testData(
            static_cast<ChibiTest*>(testObj)->testName(),
            messageType,
            message,
            param);
    else
        emit currentTestRegistry->testMessage(QString(),
                                              messageType,
                                              message,
                                              param);
}

static void ct_msleep(void *testObj, int msecs) {
    static_cast<ChibiTest*>(testObj)->msleep(msecs);
}

static void ct_set_gpio(void *testObj, int gpio, int val) {
    static_cast<ChibiTest*>(testObj)->setGpio(gpio, val);
}

static int  ct_get_gpio(void *testObj, int gpio) {
    return static_cast<ChibiTest*>(testObj)->getGpio(gpio);
}

static void ct_unexport_gpio(void *testObj, int gpio) {
    static_cast<ChibiTest*>(testObj)->unexportGpio(gpio);
}

static const QVariant & ct_get_variable_qt(void *testObj, const QVariant & key) {
    if (testObj)
        return static_cast<ChibiTest*>(testObj)->getRegistry()->getVariable(key);
    else
        return currentTestRegistry->getVariable(key);
}

static const QVariant & ct_get_global_qt(void *testObj, const QVariant & key) {
    if (testObj)
        return static_cast<ChibiTest*>(testObj)->getRegistry()->getVariable(key);
    else
        return currentTestRegistry->getGlobal(key);
}

static const FrameworkCallbacksC frameworkCallbacks = {
    /* magic */             FRAMEWORK_MAGIC_C,
    /* test_message */      ct_test_message_c,
    /* msleep */            ct_msleep,
    /* get_gpio */          ct_get_gpio,
    /* get_variable */      ct_get_variable_c,
    /* get_global */        ct_get_global_c,
};

static const FrameworkCallbacksQt frameworkCallbacksQt = {
    /* magic */             FRAMEWORK_MAGIC_QT,
    /* test_message_qt */   ct_test_message_qt,
    /* msleep */            ct_msleep,
    /* get_gpio */          ct_get_gpio,
    /* get_variable */      ct_get_variable_qt,
    /* get_global */        ct_get_global_qt,
};

ChibiTestRegistry::ChibiTestRegistry() {
    currentTestRegistry = this;
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

    if (module_c->magic == TEST_MODULE_MAGIC_C) {
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

void ChibiTestRegistry::setVariable(const QVariant &key, const QVariant &value) {
    variables.insert(key, value);
}

const QVariant & ChibiTestRegistry::getVariable(const QVariant &key) {
    return variables[key];
}

void ChibiTestRegistry::resetVariables() {
    variables.clear();
}

void ChibiTestRegistry::setGlobal(const QVariant &key, const QVariant &value) {
    globals.insert(key, value);
}

const QVariant & ChibiTestRegistry::getGlobal(const QVariant &key) {
    return globals[key];
}
