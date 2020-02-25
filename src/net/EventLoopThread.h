
#include "../thread/Condition.h"
#include <thread>
#include "../util/uncopyable.h"
#include <functional>
#include "Callbacks.h"
//class EventLoop;

class EventLoopThread : uncopyable
{
 public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop* startLoop();

 private:
  void threadFunc();
  EventLoop* loop_;
  bool exiting_;
  std::thread thread_;
  MutexLock mutex_;
  Condition cond_;
};



