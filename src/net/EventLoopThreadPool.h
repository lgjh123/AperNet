

#include "../thread/Condition.h"
#include "../thread/Mutex.h"


#include <vector>
#include <functional>
#include "../util/uncopyable.h"
#include <boost/ptr_container/ptr_vector.hpp>


class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : uncopyable
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


