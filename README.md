# learnos
学习操作系统
抛开所有光环和运气成分，靠实力生存更踏实。

### 命名空间 ns
  * newpid.c
    * 调用clone，创建新的命名空间【newpid,newuts,newns】
    * 挂载新文件系统 proc【这个暂时没有了解】
### 系统信号 sig
  * send.c
    * 给指定进程发送singal，发送sigtstp停止进程
  * sayTime.c
    * 打印时间，被停止后接收到sigcont信号后继续打印
### 网络编程 net
* addrinfo.c 
  * socket地址结构
* mytool.c
  * 打开监听socket，并且返回fd
  * connect服务器socket，并返回fd
  * 简易拆包处理
* server.c
  * 提供echo回显功能
  * 只能监听一个client
  * io多路复用监听多个client
  * 多线程监听多个client
* client.c
  * 连接server
