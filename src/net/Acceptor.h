#pragma once
#include <functional>
#include "../util/uncopyable.h"

#include <unistd.h>
#include "Channel.h"
#include "Socket.h"

class EventLoop;
class InetAddress;

class Acceptor : uncopyable
{
public:
    typedef std::function<void (int sockfd,
                                  const InetAddress&)> NewConnectionCallback;

    Acceptor(EventLoop* loop,const InetAddress& listenAddr);

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { newConnectionCallback_ = cb; }

    bool listenning() const { return listenning_; }
    void listen();

private:
    void handleRead();

    EventLoop* loop_;
    //数据成员包括Socket、Channel
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
    int ideal_; // 对于最大文件描述符的处理
};

