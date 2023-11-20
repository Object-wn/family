#ifndef __MAIN_H_
#define __MAIN_H_

#include <pthread.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

//结构体内存对齐
//#pragma pack(1) 

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

/*
@全局宏定义
@parm:  _HOST_SERVER 服务器buff大小
        _NODE_HOST 主机buff大小
        _MSG_LEN 向服务器传输数据 节点数*6  6个字节
*/
#define _HOST_SERVER 5
#define _NODE_HOST 8
#define _MSG_LEN _NODE_HOST*6
#define _EXTRACT_STR_LEN 4

//数据位是从第几位开始的
#define _DATE_HEAD 2

//提取线程和发送线程--服务器
typedef struct receive
{
       char type;//从消息队列接收消息时用于判断的消息类型
        long date[_EXTRACT_STR_LEN];
}REC;
static REC re_ex2;
REC re_ex;//放到消息队列的数据
//节点-主机收发数据结构
static  char imx_node[_NODE_HOST];

//
/*
主机进程间收发消息结构
第一位是标志位
后四位是数据位
msgsnd要求发送的数据结构要是长整型
发送数据时候计算大小要减去一个long
*/
struct msg
{
	long type;//从消息队列接收消息时用于判断的消息类型
	//long msgtype;//具体的消息类型
	unsigned char text[_HOST_SERVER];//消息正文
};

   


//互斥锁
  pthread_mutex_t mutex_node, //读取串口进程->处理线程
                mutex_host; //处理线程-> 发送线程

//线程ID
  pthread_t   id_r_dispose,   //提取线程
            id_r_sen;       //发送线程

//进程ID
  pid_t pid_dispose;
  pid_t pid_receive;

//条件变量 --和互斥锁搭配--
  pthread_cond_t  cond_node,
                cond_host;

/*
通信键值  ftok
*/
  key_t d_key;
  key_t e_key;

/*
进程消息队列键值
*/

  int d_msgid;
  int r_msgid;

//socker信息
  REC re_sock;
#define _IP_SOCK "8.130.71.224"
#define _PORT_SOCK "10000"

/***********************
 * 全局外部函数
************************/
void fork_receive();
void fork_sen();
void  * pthread_extract();
int Usart_main();
void  * pthread_sen();
int server_sock();




#endif