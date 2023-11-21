/*
@File Name: main.c
@Arouth: wn
@Version: 1.0
@Email: wn418@outlook.com
@Creat Time: 2023.11.15  9:42
@explain:imx主机  接收stm32串口信息，发送给服务器，多进程多线程实现
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include "main.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <signal.h>
#include <sys/prctl.h>
extern void fork_receive();
extern void fork_sen();
extern void  * pthread_extract();
extern int Usart_main(int argc, char *argv[]);
extern void  * pthread_sen();
extern int server_sock();
void quite();

int main(int argc, char *argv[])
{
    printf("OK PID:%d\n", getpid());
   //初始化互斥锁
    pthread_mutex_init(&mutex_node, NULL);
    pthread_mutex_init(&mutex_host, NULL);
    //pthread_mutex_init(&mutex_server, NULL);
    printf("pthread_mutex_init OK\n");

    //等待中断信号，进行退出操作
    signal(SIGINT, quite);

     //处理僵尸进程
    //子进程状态发生变化，就回收子进程
    //signal(SIGCHLD, SIG_IGN); //SIG_IGN忽略信号

    //初始化条件变量
    pthread_cond_init(&cond_host, NULL);
    pthread_cond_init(&cond_node, NULL);
    //pthread_cond_init(&cond_server, NULL);
    printf("pthread_cond_init\n");

    //创建子进程，处理提取
    if ((pid_dispose = fork()) < 0)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (0 == pid_dispose) //子进程
        {
            printf("-----------------------------fork_receive OK PID:%d\n", getpid());
            signal(SIGINT, quite);
            prctl(PR_SET_PDEATHSIG,SIGKILL);
            fork_receive(); //接收
            printf("fork_receive OK2\n");
        }
        else //父进程
        {
            printf("--------------------------------fork_sen OKPID:%d\n", getpid());
            signal(SIGINT, quite);
            //while(1)
            fork_sen(); //提取-发送
            printf("fork_sen OK2\n");
        }

   
}

/*
接收到中断信号，进行退出操作
*/
void quite()
{
    printf("in main-quite OK\n");
    //释放锁和条件变量
    pthread_mutex_destroy(&mutex_host);
    pthread_mutex_destroy(&mutex_node);
    //pthread_mutex_destroy(&mutex_server);


    //销毁条件变量
    pthread_cond_destroy(&cond_host);
    pthread_cond_destroy(&cond_node);
    //pthread_cond_destroy(&cond_server);

    //线程分离
    pthread_detach(id_r_dispose);
    pthread_detach(id_r_sen);

    //kill(pid_dispose, );

    //删除删除信号量
    semctl(d_msgid, 1, IPC_RMID);
    semctl(r_msgid, 1, IPC_RMID);

    printf("exit\n");
    exit(0);
}