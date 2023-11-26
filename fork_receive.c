/*
@File Name: fork_receive.c
@Author: wn
@Version: 1.0
@Email: wn418@outlook.com
@Creat Time: 2023.11.6 18:17
@Explain: 进程有两个线程 
            线程一 提取数据到结构体中 
                
            线程二负责发送到服务器
                
*/
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

// #define _IP_SER "43.138.87.172"
// #define _PORT "6010"

//接收兄弟线程的消息队列
//设置套接字 把接收的信息发送给服务器
//cond_host 条件
//r_msgid 消息队列键值
void *pthread_sen()
{
    printf("[fork_receivec PS] in pthread_sen\n");
    // const char *M_IP = "192.168.31.225";
    // const char *M_IP = "8.130.71.224";
    const char *M_IP = "43.138.87.172";
    const char *M_PORT = "10000";
    //socker套接字
    int sockfd;
    struct sockaddr_in serveraddr;
    FILE *fd;
    if ((fd = fopen("./t.txt", "wb")) == NULL)
    {
        printf("fopen error\n");
        return ;
    }
     if (0 > (e_key = ftok("/lib", 'a')))
    {
        perror("ftok error");
        return;
    }
     //打开套接字
    if (0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        perror("socker error");
        return ;
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(M_IP);
    serveraddr.sin_port = htons(atoi(M_PORT));

    //连接服务器
    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("connect error");
        return ;
    }

    //创建消息队列， 不存在则创建否则打开操作 | 不存在创建 否则产生错误返回
    if (-1 == (r_msgid = msgget(e_key, IPC_CREAT | IPC_EXCL | 0666))  )
    {
        //判断是否因队列存在而无法创建
        if (EEXIST == errno)
        {
            r_msgid = msgget(e_key, 0777);
        }
        else
        {
            perror("fork_dis > message > magget error");
            return;
        }
        //消息队列创建成功
        printf("[fork_receivec PS] message queue OK!\n2r_msgid:%d\n", r_msgid);
    }
    while(1)
    {
    id_r_sen = pthread_self();
//sleep(1);
     //pthread_mutex_lock(&mutex_host);
        //printf("------------get mutex_node lock1\n");
        //等待条件变量cond_node
        //无条件等待条件变量cond_node 满足后释放已经掌握的互斥锁(解锁互斥量) 
        //解锁同时申请互斥锁
        //pthread_cond_wait(&cond_host, &mutex_host);
        //printf("get cond_host lock\n");
        //printf("receive p_extrac OK\n");
        //msgrcv(d_msgid, &msgbuf2, _NODE_HOST , 1L, 0);
        printf("---------------\n");
        msgrcv(r_msgid,&re_sock,sizeof(re_sock),1L, 0);
        printf("[fork_receivec PS] rmsgid re_sock: %d\n", sizeof(re_sock));
        printf("[fork_receivec PS] 1send date: type:%c, date:%d%d%d%d%d\n",  re_sock.date[2] + '0', \
                                    re_sock.date[3], re_sock.date[4], re_sock.date[5], re_sock.date[6], re_sock.date[7]);
        // pthread_mutex_lock(&mutex_host);
        // pthread_cond_wait(&cond_host, &mutex_host);
        send(sockfd, &re_sock, sizeof(re_sock), 0);
        printf("[fork_receivec PS] 2send date: type:%d, date:%d%d%d%d%d\n",  re_sock.date[2], \
                                    re_sock.date[3], re_sock.date[4], re_sock.date[5], re_sock.date[6], re_sock.date[7]);        
        // pthread_mutex_unlock(&mutex_host);
        printf("[fork_receivec PS] sizeof: %d\n",sizeof(re_sock.date) );
        fwrite(re_sock.date, sizeof(re_sock), 5, fd);
        // pthread_mutex_lock(&mutex_host);
        // pthread_cond_wait(&cond_host, &mutex_host);
        
         //解锁  线程间共享内存
    //    pthread_mutex_unlock(&mutex_host);
    //     printf("free mutex_host2\n");
    //     pthread_cond_signal(&cond_host);
    }
}

//imx_host数组中 标志位的下标
#define _TYPE 1

//数据位的下标
#define _DATE1 2
#define _DATE2 3
#define _DATE3 4
#define _DATE4 5

struct msg msgbuf2;

void  *pthread_extract()
{
    
     if (0 > (e_key = ftok("/lib", 'a')))
    {
        perror("ftok error");
        return;
    }

    //创建消息队列， 不存在则创建否则打开操作 | 不存在创建 否则产生错误返回
    if (-1 == (r_msgid = msgget(e_key, IPC_CREAT | IPC_EXCL | 0666))  )
    {
        //判断是否因队列存在而无法创建
        if (EEXIST == errno)
        {
            r_msgid = msgget(e_key, 0777);
        }
        else
        {
            perror("fork_dis > message > magget error");
            return;
        }
        //消息队列创建成功
        printf("[fork_receivec PE] message queue OK!\n1r_msgid-----%d\n", r_msgid);
    }
/******************************************************************************************/
    

    
    id_r_dispose = pthread_self();
    REC re_ex;//放到消息队列的数据
    printf("[fork_receivec PE] PX_OK\n");
 //获取消息队键值用来发送
    if (0 > (d_key = ftok("/tmp", 'h')))
    {
        perror("ftok error");
        return;
    }

    //创建消息队列， 不存在则创建否则打开操作 | 不存在创建 否则产生错误返回
    if (-1 == (d_msgid = msgget(d_key, IPC_CREAT | IPC_EXCL | S_IRUSR|S_IWUSR))  )
    {
        //判断是否因队列存在而无法创建
        if (EEXIST == errno)
        {
            d_msgid = msgget(d_key, 0777);
        }
        else
        {
            perror("fork_dis > message > magget error");
            return;
        }
        //消息队列创建成功
        printf("[fork_receivec PE] message queue OK!\nd_msgid2:%d\n", d_msgid);
    }
    while (1)
    {
        msgrcv(d_msgid, &msgbuf2, _NODE_HOST , 1L, 0);
         LCD_main(msgbuf2.text);
        // pthread_mutex_lock(&mutex_host);
        //判断起始标志   $T00023%
        if ('$' == msgbuf2.text[_SIGN_HEAD] && '%' == msgbuf2.text[_SIGN_END])
        //放到结构体
        {
            //re_ex.F_type = msgbuf2.text[1]; //D
            //从数组提取到结构体
            re_ex.date[0] = 0; //预留位
            re_ex.date[1] = 0;
            re_ex.date[2] = msgbuf2.text[_SIGN_HEAD + 1] - '0';//H - 0
            re_ex.date[3] = msgbuf2.text[_DATE_HEAD + 0] - '0';//0
            re_ex.date[4] = msgbuf2.text[_DATE_HEAD + 1] - '0';//0
            re_ex.date[5] = msgbuf2.text[_DATE_HEAD + 2] - '0';//0
            re_ex.date[6] = msgbuf2.text[_DATE_HEAD + 3] - '0';//2
            re_ex.date[7] = msgbuf2.text[_DATE_HEAD + 4] - '0';//3

            printf("[fork_receivec PE] quantity:%d\tsize:%d\n", sizeof(re_ex.date)/sizeof(re_ex.date[0]), sizeof(re_ex.date));
            printf("[fork_receivec PE] re_ex sizeof: %d\n", sizeof(re_ex));
            LCD_main(msgbuf2.text ); //传入数组大小，复制数组到宽数组中
            //把消息添加到消息队列中
            re_ex.type = 1L;
	        if(msgsnd(r_msgid,&re_ex, sizeof(re_ex), 0) == -1)
            {
		        perror("fail to msgsnd type1");
		       // return 6;
	        }
            re_ex.type = 0;
           // printf("[fork_receivec PE] type:%c, date:%d%d%d%d%d\n", re_ex.F_type, re_ex.date[0], re_ex.date[1], re_ex.date[2], re_ex.date[3], re_ex.date[4]);
        }   printf("[fork_receivec PE] type:%d, date:%d%d%d%d%d%d\n", re_ex.date[2], re_ex.date[3], \
                                            re_ex.date[4], re_ex.date[4], re_ex.date[5], re_ex.date[6], re_ex.date[7]);
       
        //解锁 
        // pthread_cond_signal(&cond_host);
        // pthread_mutex_unlock(&mutex_host);
        // printf("[fork_receivec PE] free mutex_host1\n");
       


    }
}


//进程服务函数
void fork_sen()
{
   
    printf("[fork_receivec] input sen OK\n");
    pid_receive = getpid();
    u8 R = 0;
    //线程初始化
   if (0 != (R = pthread_cond_init(&cond_node, NULL)))
   {
        printf("pthread_cond_init error return:%d\n", R);
        R = 0;
        exit(-1);
   }
    if (0 != (R = pthread_cond_init(&cond_host, NULL)))
    {
        printf("pthread_cond_init error return:%d\n", R);
        R = 0;
        exit(-1);
    }

    //提取线程
    pthread_create(&id_r_dispose, NULL, pthread_extract, NULL);
    //发送线程
    pthread_create(&id_r_sen, NULL, pthread_sen, NULL);


    if (pthread_join(id_r_sen, NULL) != 0) {
        perror("pthread_join");
        return ;
    }
    if (pthread_join(id_r_dispose, NULL) != 0) {
        perror("pthread_join");
        return ;
    }

}

