# family
简介：
  串口接收数据，处理后发送服务器
设备：
  正点原子imx6ull-mini开发板
  阿里云服务器

main.c         客户端 创建子进程
fork_dispose.c 接收数据发送给父进程
fork_receive.c 处理数据并发送给服务器 (多线程)
server.c       服务器端 接收数据
