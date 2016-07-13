#ifndef TEST_MODULE_H_
#define TEST_MODULE_H_

#include <stdint.h>
#include <stdarg.h>

#define TEST_MODULE_MAGIC_C     0x0592d523
#define TEST_MODULE_MAGIC_QT    0x0592d524

#define FRAMEWORK_MAGIC_QT 0x8f24b842
#define FRAMEWORK_MAGIC_C 0x8f24b843

typedef enum test_message_type {
    InfoMessage,    // Non-error informational message.
    ErrorMessage,   // Error message.  Also indicates an error occurred.
    DebugMessage,   // Debug message, may be hidden
    SetHeader,      // Set the window header / banner message
    TestPass,       // Indicate that the test passed
    StartTests,     // Used to start the tests from the beginning
    SetGpio,        // Sets a GPIO to "export", and sets its value to 1
    ClearGpio,      // Exports a GPIO and sets its value to 0
    UnexportGpio,   // Undoes the result of an "export"
    TestingPoint,   // Indes of the current (x, y) under test
    PassPoint,      // Indicates a specific (x, y) passed
    FailPoint,      // Indicates a specific (x, y) failed
} TestMessageType;

typedef struct test_instance TestInstance;

#ifdef __cplusplus

#include <QString>
#include <QVariant>
#include <QMap>

typedef struct framework_callbacks_qt {
  uint32_t magic;
  void (*send_message)(void *testObj, TestMessageType messageType, const QVariant & message);
  void (*msleep)(void *testObj, int msecs);
  int  (*get_gpio)(void *testObj, int gpio);
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

typedef struct framework_callbacks {
  uint32_t magic;
  void (*send_message)(void *testObj, TestMessageType messageType, const void *message);
  void (*msleep)(void *testObj, int msecs);
  int  (*get_gpio)(void *testObj, int gpio);
} FrameworkCallbacks;

typedef struct test_module_c {
  uint32_t          magic;
  void            (*module_init)(const FrameworkCallbacks *callbacks);
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
#endif /* TEST_MODULE_H_ */
