#include "../net/TcpServer.h"
#include "../util/uncopyable.h"

class HttpRequest;
class HttpResponse;

class HttpServer : uncopyable
{
 public:

  HttpServer(EventLoop* loop,
             const InetAddress& listenAddr);


  void setResponse(const HttpRequest& req,HttpResponse* resp);

  void setThreadNum(int numThreads)
  {
    server_.setThreadNum(numThreads);
  }

  void start();

 private:
  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 int receiveTime);
  void onRequest(const TcpConnectionPtr&, const HttpRequest&);

  TcpServer server_;
};
