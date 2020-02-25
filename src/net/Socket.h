#pragma once

#include "../util/uncopyable.h"

class InetAddress;

class Socket : uncopyable
{
public:
    explicit Socket(int sockfd)
        :sockfd_(sockfd)
    {  }

    ~Socket();
    
    int fd() const { return sockfd_; }
    //如果address被使用出abord
    void bindAddress(const InetAddress& localaddr);

    void listen();

    int accept(InetAddress* peeraddr);

    //Enable/disable SO_REUSEADDR ??
    void setReuseAddr(bool on);

    void shutdownWrite();

private:

    const int sockfd_;
};

