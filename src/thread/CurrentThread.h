
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>



namespace CurrentThread
{
  pid_t tid();
  const char* name();
  bool isMainThread();
}


