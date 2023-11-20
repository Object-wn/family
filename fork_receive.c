/*
@File Name: fork_receive.c
@Author: wn
@Version: 1.0
@Email: wn418@outlook.com
@Creat Time: 2023.11.6 18:17
@Explain: 进程有两个线程 
            线程一 提取数据到结构体中 
                使用互斥锁、提取另一个进程放进消息队列的数据
                然后通过共享内存给另一个线程传递数据
                发送信号给另一个线程 可以读取数据了
            线程二负责发送到服务器
                接收到线程一的消息 读取共享内存的数据 （结构体）
                通过socker套接字发送给服务器
*/

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <errno.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
       #include <sys/ipc.h>
       #include <sys/msg.h>

static  int sockfd;
static  struct sockaddr_in str_server;

//接收兄弟线程的消息队列
//设置套接字 把接收的信息发送给服务器
//cond_host 条件
//r_msgid 消息队列键值
void *pthread_sen()
{
    while(1)
    {
    id_r_sen = pthread_self();
    // if (server_sock() != 0)
    // {

    // }

     pthread_mutex_lock(&mutex_host);
        //printf("------------get mutex_node lock1\n");
        //等待条件变量cond_node
        //无条件等待条件变量cond_node 满足后释放已经掌握的互斥锁(解锁互斥量) 
        //解锁同时申请互斥锁
        pthread_cond_wait(&cond_host, &mutex_host);
        printf("get mutex_host lock\n");
        printf("receive p_extrac OK\n");

        if(msgrcv(r_msgid,&re_sock,sizeof(re_sock) - sizeof(long),0, 0) == -1){
		perror("fail to msgsnd type2");
		//exit(1);
	}
        printf("-------------%d---------------\n", re_sock.type);

         //解锁  线程间共享内存
        pthread_mutex_destroy(&mutex_host);
        pthread_cond_signal(&cond_host);
    }
}
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdio.h>
#include "main.h"
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

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

    int fd;
    if (-1 == (fd = open("./t.txt", O_RDWR|O_CREAT,0600)))
    {
        perror("open error");
        exit(-1);
    }


    id_r_dispose = pthread_self();
    REC re_ex;//放到消息队列的数据
    printf("PX_OK\n");
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
        printf("message queue OK!-^^^^^^^^^^^^^^^^^^d_msgid2:%d\n", d_msgid);
    }
    while (1)
    {
        
        // msgsnd(r_msgid, &msgbuf, sizeof(msgbuf) - sizeof(long), 0);
        // printf("read lock\n");
        /****************************
        有可能这里出错    阻塞等待兄弟进程放锁
        ***************************/
       // pthread_mutex_lock(&mutex_node);
        printf("#########get mutex_node lock\n");
        //等待条件变量cond_node
        //无条件等待条件变量cond_node 满足后释放已经掌握的互斥锁(解锁互斥量) 
        //解锁同时申请互斥锁
        //pthread_cond_wait(&cond_node, &mutex_node);
        printf("get mutex_node lock2\n");
        printf("receive d_msgid OK\n");
        //清零imx_host2空间
        bzero(msgbuf2.text, sizeof(msgbuf2.text) - 1);
        printf("wait form fork_dispose--\n");
        //sleep(1);
        //从消息队列获取消息 获取消息队列第一条类型为long的数据
        printf("=+++==+=+++++++=======%d\n", d_msgid);
        msgrcv(d_msgid, &msgbuf2,20 , 1L, 0);
        //放到结构体
        //re_ex.type = msgbuf2.text[1];
        //从数组提取到结构体
        /**********************
         * 下边那个发送也会使用结构体，但是结构体已经被锁住了，所以下边那个发送很有可能会出问题 在此标记
         * ***********************/
        // re_ex.date[_DATE1 - _DATE_HEAD] = msgbuf2.text[_DATE1];
        // re_ex.date[_DATE2 - _DATE_HEAD] = msgbuf2.text[_DATE2];
        // re_ex.date[_DATE3 - _DATE_HEAD] = msgbuf2.text[_DATE3];
        // re_ex.date[_DATE4 - _DATE_HEAD] = msgbuf2.text[_DATE4];

        // re_ex.date[0] = msgbuf2.text[2];
        // re_ex.date[1] = msgbuf2.text[3];
        // re_ex.date[2] = msgbuf2.text[4];
        // re_ex.date[3] = msgbuf2.text[5];
        //re_ex.date[4] = 0;
        write(fd, msgbuf2.text, 8);
        printf("re_ex~~~~~~~~~~~~~~~~~~~~~~~~~~~%s\n", re_ex);
        //pthread_cond_signal(&cond_node);
        //解锁 对应pthread_cond_wait
        //pthread_mutex_unlock(&mutex_node);
        //printf("free mutex_node2\n");
        //printf("re_ex:%s\n", msgbuf2.text);
        

       
        //printf("successful extracted  re_ex\n");

/****************************************************************************************/

        //互斥锁实现 数据放到消息队列 给发送线程
        //上锁
        pthread_mutex_lock(&mutex_host);
        
        printf("set mutex_host lock OK!\n");

        //把消息添加到消息队列中
        re_ex2.type = 1L;
	    if(msgsnd(r_msgid,&re_ex2, sizeof(re_ex), 0) == -1)
        {
		    perror("fail to msgsnd type2");
		   // return 6;
	    }
pthread_cond_signal(&cond_host);
        //解锁  线程间共享内存
        pthread_mutex_unlock(&mutex_host);
        printf("free lock2\n");
        


    }
}


//进程服务函数
void fork_sen()
{
    if (0 > (e_key = ftok("/home", 'a')))
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
        printf("message queue OK!2222    d_msgid-----%d\n", r_msgid);
    }

    printf("input sen OK\n");
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

