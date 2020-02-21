#pragma once
#include <functional>
#include <memory>

//#include "datetime/Timestamp.h"

class EventLoop;
//客户端!!定义回调
class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&,
                             Buffer* buf,
                             int Timestamp)> MessageCallback;
                             ///!!!!!FIXME XXXX

typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void(EventLoop*)> threadFuncType;