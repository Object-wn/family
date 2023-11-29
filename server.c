/****
 * @File Name: server.c
 * @Author: Wn
 * @Version: 1.o
 * @Email: wn418@outlook.com
 * @Creat Time: 2023.11.21  20:24
 * @explain: 接收socker信息
*/

#include <stdio.h>
#include <stdlib.h>
 #include <strings.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#define _EXTRACT_STR_LEN 8 //2位预留位 1位标志位 5位数据位 

//提取线程和发送线程--服务器
typedef struct receive
{
      char type;//从消息队列接收消息时用于判断的消息类型
      int date[_EXTRACT_STR_LEN];
}REC;

/*
struct tm t;
    time_t sec;

    sec = time(NULL);
    if (sec == -1)
    {
        perror("time error");
        return 1;
    }

    localtime_r(&sec, &t);
    printf(" %d year%d month%d day %d:%d:%d\n",
    t.tm_year + 1900, t.tm_mon, t.tm_mday,
    t.tm_hour, t.tm_min, t.tm_sec);
*/

int do_client(int acceptfd)
{
    //保存时间数据
    time_t time_date = time(NULL);
    struct tm local_t;
    REC rec;
    printf("successfu locnnection\n");
    // while (0 < recv(acceptfd, &rec, sizeof(rec), 0))
    while (0 < recv(acceptfd, &rec, sizeof(rec), 0))
    {
        //获取当前时间
        //localtime_r(&time_date, &local_t);
        printf("type:%c\n", rec.date[2] + '0');
        // printf("1 %d year%d month%d day %d:%d:%d\n",  //本地时间
        // local_t.tm_year + 1900, local_t.tm_mon + 1, local_t.tm_mday,
        // local_t.tm_hour, local_t.tm_min, local_t.tm_sec);
        switch(1)
        {
            case 1:
            send(acceptfd, "O", 1, 0);
            localtime_r(&time_date, &local_t);
            printf("time:  %d year%d month%d day %d:%d:%d\n",  //本地时间
                local_t.tm_year + 1900, local_t.tm_mon + 1, local_t.tm_mday,
                local_t.tm_hour, local_t.tm_min, local_t.tm_sec);
                //printf("date0-2:%d\n", rec.date[2]);
            printf("date3-7:%d%d%d%d%d\n", rec.date[3], rec.date[4], rec.date[5], rec.date[6], rec.date[7]);
        }
        time_date = time(NULL);
    }
     //printf("type:%c\n", rec.F_type);
    printf("client exit\n");
    close(acceptfd);
    exit(EXIT_SUCCESS);
    return 0;
}


int main(int argc, char *argv[])
{
    //socker套接字
    int sockfd;
    struct sockaddr_in serveraddr;
    REC rec; //接收数据  与客户端格式相同
    int acceptfd;
    int pid;
    
    //判断参数
    if (argc != 3)
    {
        printf("input error\n");
        exit(-1);
    }

    if (0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        perror("server socket error");
        exit(-1);
    }
    
        printf("socket OK!\n");
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    //将套接字与IP相连接
    if (0 > bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)))
    {
        perror("bind error");
        exit(-1);
    }
    
        printf("bind OK!\n");
    //设置套接字为监听模式
    if (0 > listen(sockfd, 5))
    {
        printf("listen fail\n");
        exit(-1);
    }
    
        printf("listen OK!\n");
    //处理僵尸进程
    //子进程状态发生变化，就回收子进程
    signal(SIGCHLD, SIG_IGN); //SIG_IGN忽略信号
    printf("signal OK!\n");
    while(1)
    {
        printf("while OK!\n");
        //获取连接套接字，端口号与这个套接字连接
        if (0 > (acceptfd = accept(sockfd, NULL, NULL))) //获取客户端的连接请求并建立连接
        {
            perror("accept fail");
            exit(-1);
        }
        printf("accept OK!\n");
        if (0 > (pid = fork()))
        {
            perror("fork fail");
            exit(-1);
        }
        else if (0 == pid) //子进程
        {
            //子进程不需要监听，关闭监听套接字
            close(sockfd);
            printf("fork\n");
            //处理客户端具体消息
            do_client(acceptfd);
        }
        else //父进程
        {
            //父进程不需要客户端的接收套接字acceptfd, 节省资源，直接关闭
            close(acceptfd);
        }
    }
    return 0;
}