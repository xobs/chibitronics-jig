#include "chibitest.h"
#include "chibitestregistry.h"

static void ct_test_message(void *testObj, TestMessageType messageType,
                            int value, const char *message) {
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
  /* test_message */  ct_test_message,
  /* msleep */        ct_msleep,
  /* set_gpio */      ct_set_gpio,
  /* get_gpio */      ct_get_gpio,
  /* unexport_gpio */ ct_unexport_gpio,
};


ChibiTestRegistry::ChibiTestRegistry() {
}

void ChibiTestRegistry::addModule(const struct test_module *module) {
    module->module_init(&frameworkCallbacks);
    registry.insert(module->module_name,
                    QVariant::fromValue((void *)module));
}

const TestModule *ChibiTestRegistry::getModule(const QString &name) {
    return (const TestModule *)registry.value(name).value<void *>();
}
