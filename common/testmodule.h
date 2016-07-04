#ifndef TEST_MODULE_H_
#define TEST_MODULE_H_

#include <stdint.h>
#include <stdarg.h>

class QString;

#ifdef __cplusplus
extern "C" {
#endif

typedef enum test_message_type {
  infoMessageType,
  errorMessageType,
  debugMessageType,
  setHeaderType,
  setStickerNumType,
  testPassType,
} TestMessageType;

typedef struct test_instance TestInstance;
class QString;

typedef struct framework_callbacks {
  void (*test_message)(void *testObj, TestMessageType messageType,
                       int value, const char *message);
  void (*test_message_qt)(void *testObj, TestMessageType messageType,
                          int value, const QString *message);
  void (*msleep)(void *testObj, int msecs);
  void (*set_gpio)(void *testObj, int gpio, int val);
  int  (*get_gpio)(void *testObj, int gpio);
  void (*unexport_gpio)(void *testObj, int gpio);
} FrameworkCallbacks;

#define TEST_MODULE_MAGIC 0x0592d523

typedef struct test_module {
  uint32_t          magic;
  void            (*module_init)(const FrameworkCallbacks *callbacks);
  const char *    module_name;
  const char *    module_description;
  TestInstance *  (*instance_init)(void *testObj, va_list ap);
  const char *    (*instance_name)(TestInstance *intsance);
  const QString & (*instance_name_qt)(TestInstance *instance);
  void            (*instance_run)(TestInstance *instance);
  void            (*instance_free)(TestInstance *instance);
} TestModule;

extern TestModule test_module;

#ifdef __cplusplus
};
#endif
#endif /* TEST_MODULE_H_ */
