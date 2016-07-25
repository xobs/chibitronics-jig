#ifndef TEST_MODULE_H_
#define TEST_MODULE_H_

#include <stdint.h>
#include <stdarg.h>

#define TEST_MODULE_MAGIC_C     0x0592d523
#define TEST_MODULE_MAGIC_QT    0x0592d524

#define FRAMEWORK_MAGIC_QT      0x8f24b842
#define FRAMEWORK_MAGIC_C       0x8f24b843

typedef enum test_message_type {
    InfoMessage,    // 0  / Non-error informational message.
    ErrorMessage,   // 1  / Error message.  Also indicates an error occurred.
    DebugMessage,   // 2  / Debug message, may be hidden
    FatalMessage,   // 3  / An error message that indicates we can't continue at all.
    SetHeader,      // 4  / Set the window header / banner message
    TestPass,       // 5  / Indicate that the test passed
    StartTests,     // 6  / Used to start the tests from the beginning
    SetGpio,        // 7  / Sets a GPIO to "export", and sets its value to 1
    ClearGpio,      // 8  / Exports a GPIO and sets its value to 0
    UnexportGpio,   // 9  / Undoes the result of an "export"
    TestingPoint,   // 10 / Indes of the current (x, y) under test
    PassPoint,      // 11 / Indicates a specific (x, y) passed
    FailPoint,      // 12 / Indicates a specific (x, y) failed
    SetVariable,    // 13 / Sets a variable within the test infrastructure's current run
    SetGlobal,      // 14 / Sets a persistent variable that is not dependent on the current run
} TestMessageType;

typedef struct test_instance TestInstance;

#ifdef __cplusplus

#include <QString>
#include <QVariant>
#include <QMap>

typedef struct framework_callbacks_qt {
    uint32_t magic;
    void (*send_message)(void *testObj, TestMessageType messageType, const QVariant & message, const QVariant & parameter);
    void (*msleep)(void *testObj, int msecs);
    int  (*get_gpio)(void *testObj, int gpio);
    const QVariant & (*get_variable)(void *testObj, const QVariant & key);
    const QVariant & (*get_global)(void *testObj, const QVariant & key);
} FrameworkCallbacksQt;

typedef struct test_module_qt {
    uint32_t          magic;
    void            (*module_init)(const FrameworkCallbacksQt *callbacks);
    const QString & module_name;
    const QString & module_description;
    TestInstance *  (*instance_init)(void *testObj, QMap<QString, QVariant> params);
    const QString & (*instance_name)(TestInstance *instance);
    void            (*instance_run)(TestInstance *instance);
    void            (*instance_free)(TestInstance *instance);
} TestModuleQt;

extern "C" {
#endif

typedef struct framework_callbacks_c {
  uint32_t magic;
  void (*send_message)(void *testObj, TestMessageType messageType, const char *message, const char *parameter);
  void (*msleep)(void *testObj, int msecs);
  int  (*get_gpio)(void *testObj, int gpio);
  const char * (*get_variable)(void *testObj, const char * key);
  const char * (*get_global)(void *testObj, const char * key);
} FrameworkCallbacksC;

typedef struct test_module_c {
  uint32_t          magic;
  void            (*module_init)(const FrameworkCallbacksC *callbacks);
  const char *    module_name;
  const char *    module_description;
  TestInstance *  (*instance_init)(void *testObj, va_list ap);
  const char *    (*instance_name)(TestInstance *intsance);
  void            (*instance_run)(TestInstance *instance);
  void            (*instance_free)(TestInstance *instance);
} TestModuleC;

#ifdef __cplusplus
};

typedef union test_module {
    TestModuleC c;
    TestModuleQt qt;
} TestModule;
#endif

#ifndef Q_DECL_EXPORT
#define Q_DECL_EXPORT
#endif

#endif /* TEST_MODULE_H_ */
