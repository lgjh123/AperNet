#include "TcpServer.h"

//#include "logging/Logging.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "SocketsOps.h"
#include "EventLoopThreadPool.h"


#include <functional>
#include <stdio.h> //snprintf
#include <iostream>

TcpServer::TcpServer(EventLoop* loop,const InetAddress& listenAddr)
    :loop_(loop),    //check out null
    name_(listenAddr.toHostPort()),
    acceptor_(new Acceptor(loop,listenAddr)),
    threadPool_(new EventLoopThreadPool(loop)),
    started_(false),
    nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(
                                std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
    //将Acceptor的setNew...绑定到TcpServer的new....
    //由TcpServer 接管
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
    if(!started_)
    {
        started_ = true;
        threadPool_->start();
    }

    if(!acceptor_->listenning())
    {
        loop_->runInLoop(
                std::bind(&Acceptor::listen,acceptor_.get()));
       //如果没有listen被server接管 
    }
}

void TcpServer::newConnection(int sockfd,const InetAddress& peerAddr)
{
    //printf("TcpServer:::::newConnnnnnnnn\n");
    loop_->assertInLoopThread();
    char buf[32];
    snprintf(buf,sizeof buf,"#%d",nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;

    //std::cout << "TcpServer::newConnection [" << name_
    //       << "] - new connection [" << connName
    //       << "] from " << peerAddr.toHostPort() 
    //      << "fd = " << sockfd << std::endl;
        
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    //FIXME ??
    EventLoop* ioLoop = threadPool_->getNextLoop();
    TcpConnectionPtr conn(
        std::make_shared<TcpConnection>(ioLoop,connName,sockfd,localAddr,peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);             //这两个callback是从httpserver传来
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection,this,std::placeholders::_1));
    // conn->connectEstablished();
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished,conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this,conn));
}
void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
   // std::cout<< "Tcpserver::removeConnectionInLoop [" << name_ 
    //            << "] - connection " << conn->name() << std::endl;
    size_t n = connections_.erase(conn->name());
    assert( n == 1); (void)n;
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed,conn));

}
void TcpServer::setThreadNum(int numThreads)
{
  assert(0 <= numThreads);
  threadPool_->setThreadNum(numThreads);
}
