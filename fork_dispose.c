/*
@File Name: fork_dispose.c
@Author: wn
@Version: 1.0
@Email: wn418@outlook.com
@Creat Time: 2023.11.5  9:42
@explain:处理stm32发送的数据，通过消息队列传递给发送进程
*/

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <errno.h>

// //节点-主机收发数据结构
// char imx_node[_NODE_HOST];
struct msg msgbuf;
//对应Usart_main()所需要的参数 读取
char *ARGV[] = {"--dev=/dev/ttymxc2",
                "--type=read"};
//串口返回错误代码
u8 Usart_main_error;
//数据提取返回错误代码
u8 message_queue_error;

 int message_queue();

/*
提取数据类型和数据内容
放入imx_host[]
@parameter: 节点-主机互斥锁   
*/
void fork_receive()
{
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
        printf("message queue OK!<<<<<<<<<<<<<<<<<<<<<<<<<d_msgid1:%d\n", d_msgid);
    }
    //printf("input receive OK\n");

    pid_dispose = getpid();
    while (1)
    {
        
    //串口读取数据 --加锁--  锁在main中初始化完成
    //pthread_mutex_lock(&mutex_node);
    //pthread_cond_wait(&cond_node, &mutex_node);
    //printf("get mutex_node1\n");
   // printf("fork_receive read usart\n");


    //Usart_main_error = Usart_main(3, &ARGV);


    if (0 != (Usart_main_error = Usart_main(3, &ARGV)) ) //imx_host[_NODE_HOST];数组中已经有数据了
    {
        fprintf(stderr, "Usart_main error:%d\n", Usart_main_error);
        //返回错误，代码另设意思
        //return ;
    }

        printf("[fork_disopose] magbuf: %s\td_msgid: %d\n", msgbuf.text, d_msgid);

    if (0 != (message_queue_error = message_queue()))
    {
        fprintf(stderr, "message_queue error:%d\n", message_queue_error);
        //返回错误，返回值另设意思
        //return ;
    }

    
    }
}

/*
进程通信用消息队列，放入imx_host数组中通过消息队列由另一个进程读取
@return:    0
            1 ftok错误
            2 创建消息队列错误
            3 添加数据到消息队列错误
*/
 int message_queue()
{
    //获取消息队键值用来发送
    // if (0 > (d_key = ftok("/tmp", 'g')))
    // {
    //     perror("ftok error");
    //     return 1;
    // }
    //printf("in message_queue\n");
    
        //添加数据到消息队列中
        msgbuf.type = 1L;
        printf("[fork_disopose] msgsnd type: %d\ttext:%s\n", msgbuf.type, msgbuf.text);
        printf("[fork_disopose] magbuf_Size: %d\ttext_size:%d\n", sizeof(msgbuf), sizeof(msgbuf.text));
        msgsnd(d_msgid, &msgbuf, _NODE_HOST, 0);
        // if (msgsnd(d_msgid, &msgbuf, sizeof(msgbuf) - sizeof(long), 0) == -1)
        // {
        //     printf("msgid:%d\n", d_msgid);
        //     printf("###imx_host %s\n", msgbuf.text);
        //     perror("fork_dis > message > msgsnd fild2");
        //     //return 3;
        // }
        msgbuf.type = 0;
        
        //printf("successfully added d_msgid\n");

    //}
    return 0;
}