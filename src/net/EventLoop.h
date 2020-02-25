#pragma once
#include <memory>
#include "../util/uncopyable.h"
#include <vector>
#include <sys/types.h>
#include <sys/syscall.h>
#include <mutex>
#include "Callbacks.h"
#include <functional>
#include "../thread/Mutex.h"
#include "../thread/CurrentThread.h"

class Channel;
class EPoller;

class EventLoop : uncopyable
{
public:
    typedef std::function<void()> Functor;
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();

    void runInLoop(const Functor& cb);
    //在loop中调用设定的回调函数如果用户在当前io线程调用
    //则会同步执行回调，其他线程调用这个函数，则会先加入任务队列
    //io线程会被唤醒，调用这个回调
    //有了这个函数就可以在“线程间调配任务”
    //为io线程平时阻塞在loop中，为了立即执行函数回调。
    //向fd上写一个字节??
    //使用Linux中的eventfd(2)??不用管缓冲区
    void queueInLoop(const Functor& cb);
    //结束loop后调用queue
    void wakeup();


    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

     void assertInLoopThread()
    {
        if(!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }
    bool isInLoopThread() const {return threadId_ == CurrentThread::tid();} 

private:
    void abortNotInLoopThread();

    void handleRead(); //用于wake up
    void doPendingFunctors();
    int wakeupFd_;
    std::vector<Functor> pendingFunctors_;
    std::unique_ptr<Channel> wakeupChannel_;

    std::unique_ptr<EPoller> poller_;
    typedef std::vector<Channel*> ChannelList;
    
    bool looping_;
    bool quit_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    ChannelList activeChannels_;
    MutexLock mutex_;
};

