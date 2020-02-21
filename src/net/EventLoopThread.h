
#include "../thread/Condition.h"
#include <thread>
#include <boost/noncopyable.hpp>
#include <functional>
#include "Callbacks.h"
//class EventLoop;

class EventLoopThread : boost::noncopyable
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



