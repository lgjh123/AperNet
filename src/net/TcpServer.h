#pragma once
//TCPserver class 的功能是管理accept(2)获得的Tcpconnection
//TcpServer是提供给用户直接使用的，生命周期由用户控制
//用户只需设置好callback，在调用start()就好


#include "Callbacks.h"
#include "TcpConnection.h"

#include <map>
#include "../util/uncopyable.h"

class Acceptor;
class EventLoop;
class EventLoopThreadPool;
class TcpServer : uncopyable
{
public:
    TcpServer(EventLoop* loop,const InetAddress& listenAddr);
   
    ~TcpServer();

    //线程安全，可以多次调用？
    //如果没有监听套接字，开启server
    void start();

    //设置回调函数（非线程安全）
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; };
    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; };

    void setThreadNum(int numThreads);

private:
    //不是线程安全的但是，只在loop中执行
    void newConnection(int sockfd,const InetAddress& peerAddr);
    //线程安全
    void removeConnection(const TcpConnectionPtr& conn);
    //不是线程安全但是在loop中执行
    void removeConnectionInLoop(const TcpConnectionPtr& conn);

    typedef std::map<std::string,TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_; //避免暴露accept
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    bool started_;
    int nextConnId_;  //在Eventloop线程
    ConnectionMap connections_;

    std::unique_ptr<EventLoopThreadPool> threadPool_;
};

