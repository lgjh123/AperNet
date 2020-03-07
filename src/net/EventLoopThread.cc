
#include "EventLoopThread.h"
#include "../http/HttpServer.h"
#include "EventLoop.h"

#include <functional>

EventLoopThread::EventLoopThread()
  : loop_(NULL),
    exiting_(false),
    mutex_(),
    cond_(mutex_)
{
}

EventLoopThread::~EventLoopThread()
{
  exiting_ = true;
  loop_->quit();
  thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
  
  thread_ = std::thread(&EventLoopThread::threadFunc,this);

  {
    MutexLockGuard lock(mutex_);
    while (loop_ == NULL)
    {
      cond_.wait();
    }
  }

  return loop_;
}

void EventLoopThread::threadFunc()
{
  //EventLoop loop
  EventLoop loop;
  HttpServer server(&loop, InetAddress(8000));
  server.start();

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();
  //assert(exiting_);
}

