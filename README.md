# AperNet

AperNet (v0.08)

## 介绍

AperNet 是一个基于主-从 Reactor 模型的多线程异步 Web 服务器。支持处理 HTTP/1.1 静态 GET 请求。

## 特点

* 高性能多线程模型：采用 Oneloop per thread 的线程模型,每一个线程都有一个 EventLoop。主线程负责分发事件(RoundRobin),每一个线程都有一个 EventPoll 来接受并处理时间。
* 防止内存泄漏：使用了智能指针管理对象周期。
* 异步事件的实现：采用的 bind()+function()来实现。
* 解决锁冲突：任务回调非当前 I/O 线程不会执行,暂存在队列任务队列中,使用 eventfd(2)唤醒当前 I/O 执行。
* 提高相应速度：使用了 non-blocking I/O+应用层 Buffer。
* 支持管线化：使用状态机解析HTTP请求
* 支持长连接: 使用std::map储存fd与channel(事件分发器)的对应关系
* 支持优雅关闭 

## 使用方法

安装前，你需要确保你的机器已经正确配置了cmake 3.0以上的版本，你可在软件源中下载并安装cmake。

```cd src```
```cmake .```
```./server```

## 性能测试

```$ab  ab -n 500000 -r -c 512 http://127.0.0.1:8000/hello.html```

#### AperNet

```shell
Concurrency Level:      512
Time taken for tests:   18.925 seconds
Complete requests:      500000
Failed requests:        0
Non-2xx responses:      500000
Requests per second:    26419.50 [#/sec] (mean)
Time per request:       0.038 [ms] (mean, across all concurrent requests)
```

#### Apach

```shell
Concurrency Level:      512
Time taken for tests:   22.413 seconds
Complete requests:      500000
Failed requests:        0
Non-2xx responses:      500000
Requests per second:    22308.63 [#/sec] (mean)
Time per request:       0.045 [ms] (mean, across all concurrent requests)
```

```$webbench http://127.0.0.1:8000/ -2 --get -c 1000```

```shell
Speed=2437418 pages/min, 6662243 bytes/sec.
```

# 性能分析

```Total time```代表函数的调用总时间(包括函数内的调用)
```Self time```代表函数调用的时间(不包括其它函数)
按照selftime排序：

```shell
 Total time   Self time       Calls  Function
  ==========  ==========  ==========  =================================================
    2.055  m    1.030  m        3004  EPoller::poll
    1.025  m    1.025  m        2987  epoll_wait
   11.298 ms   11.298 ms        2000  write
    6.444 ms    6.444 ms      167030  std::forward
```

按照totaltime排序：

```shell
 Total time   Self time       Calls  Function
  ==========  ==========  ==========  =================================================
 475.619 ms    2.053 ms        7499  std::function::operator ()
  473.174 ms    1.964 ms        7499  std::_Function_handler::_M_invoke
  468.986 ms    1.909 ms        7499  std::_Bind::operator ()
  464.056 ms    2.579 ms        7499  std::_Bind::__call
  213.488 ms  618.605 us        2998  Channel::handleEvent
```

**可以看到server主要的性能消耗在于I/O(write)和函数回调，符合预期。**
