#ifndef TEST_MODULE_H_
#define TEST_MODULE_H_

#include <stdint.h>
#include <stdarg.h>

#define TEST_MODULE_MAGIC_C     0x0592d523
#define TEST_MODULE_MAGIC_QT    0x0592d524

#define FRAMEWORK_MAGIC_QT 0x8f24b842
#define FRAMEWORK_MAGIC_C 0x8f24b843

typedef enum test_message_type {
  infoMessageType,      // Non-error informational message.
  errorMessageType,     // Error message.  Also indicates an error occurred.
  debugMessageType,     // Debug message, may be hidden
  setHeaderType,
  setStickerNumType,
  testPassType,         // Indicate that the test passed
  startTestsType,       // Used to start the tests from the beginning
} TestMessageType;

typedef struct test_instance TestInstance;

#ifdef __cplusplus

#include <QString>
#include <QVariant>
#include <QMap>

typedef struct framework_callbacks_qt {
  uint32_t magic;
  void (*send_message)(void *testObj, TestMessageType messageType,
                          int value, const QString &message);
  void (*msleep)(void *testObj, int msecs);
  void (*set_gpio)(void *testObj, int gpio, int val);
  int  (*get_gpio)(void *testObj, int gpio);
  void (*unexport_gpio)(void *testObj, int gpio);
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
  void (*send_message)(void *testObj, TestMessageType messageType,
                       int value, const char *message);
  void (*msleep)(void *testObj, int msecs);
  void (*set_gpio)(void *testObj, int gpio, int val);
  int  (*get_gpio)(void *testObj, int gpio);
  void (*unexport_gpio)(void *testObj, int gpio);
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
