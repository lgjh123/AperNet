# AperNet

AperNet (v0.08)

##介绍

AperNet 是一个基于主-从 Reactor 模型的多线程异步 Web 服务器。支持处理 HTTP/1.1 静态 GET 请求。

## 特点

* 多线程模型：采用 Oneloop per thread 的线程模型,每一个线程都有一个 EventLoop。主线程负责分发事件(RoundRobin),每一个线程都有一个 EventPoll 来接受并处理时间。
* 防止内存泄漏：使用了智能指针管理对象周期。
* 异步事件的实现：采用的 bind()+function()来实现。
* 解决锁冲突：任务回调非当前 I/O 线程不会执行,暂存在队列任务队列中,使用 eventfd(2)唤醒当前 I/O 执行。

* 提高相应速度：使用了 non-blocking I/O+应用层 Buffer。
* 支持管线化：使用状态机解析HTTP请求

##使用方法

安装前，你需要确保你的机器已经正确配置了cmake 3.0以上的版本，你可在软件源中下载并安装cmake。

```cd src```
```cmake .```
```./server```

## 性能测试



