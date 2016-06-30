#ifndef TEST_MODULE_H_
#define TEST_MODULE_H_

#include <stdint.h>
#include <stdarg.h>

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

typedef struct framework_callbacks {
  void (*test_message)(void *testObj, TestMessageType messageType,
                       int value, const char *message);
  void (*msleep)(void *testObj, int msecs);
  void (*set_gpio)(void *testObj, int gpio, int val);
  int  (*get_gpio)(void *testObj, int gpio);
  void (*unexport_gpio)(void *testObj, int gpio);
} FrameworkCallbacks;

typedef struct test_module {
  const char       *name;
  const char       *description;
  void            (*module_init)(const FrameworkCallbacks *callbacks);
  TestInstance *  (*instance_init)(void *testObj, va_list ap);
  void            (*instance_run)(TestInstance *instance);
  const char *    (*instance_status_str)(TestInstance *instance);
  int             (*instance_status)(TestInstance *instance);
  void            (*instance_free)(TestInstance *instance);
} TestModule;


#ifdef __cplusplus
};
#endif
#endif /* TEST_MODULE_H_ */
