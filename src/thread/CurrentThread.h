
#include "../util/uncopyable.h"
#include <memory>
#include <pthread.h>



namespace CurrentThread
{
  pid_t tid();
  const char* name();
  bool isMainThread();
}


