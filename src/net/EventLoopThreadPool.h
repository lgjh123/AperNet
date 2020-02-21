

#include "../thread/Condition.h"
#include "../thread/Mutex.h"
//#include "../thread/Thread.h"


#include <vector>
#include <functional>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>


class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : boost::noncopyable
{
 public:
  EventLoopThreadPool(EventLoop* baseLoop);
  ~EventLoopThreadPool();
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }
  void start();
  EventLoop* getNextLoop();

 private:
  EventLoop* baseLoop_;
  bool started_;
  int numThreads_;
  int next_;  // always in loop thread
  boost::ptr_vector<EventLoopThread> threads_;
  std::vector<EventLoop*> loops_;
};


