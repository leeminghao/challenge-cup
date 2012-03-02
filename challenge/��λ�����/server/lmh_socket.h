/*
 *============================================================================
 *                  2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_socket.h
 * 文件摘要：本头文件用于声明系统使用socket传输数据的接口
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/24
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *============================================================================
 */

#ifndef LMH_SOCKET_H
#define LMH_SOCKET_H

/* ISO c headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

/* Linux c headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* 一些限制 */
#define MAX_LISTEN     20	/* 允许的最大连接请求数 */
#define PER_MAX       512	/* 每次传送的数据大小不超过的字节数 */
#define TEMP_SIZE      10	/* 温度数据的长度 */

/* 客户端请求类型 */
#define VIDEO       0   /* 请求视频数据 */
#define TEMP_HUM    1	/* 请求温湿度数据 */
#define OPEN_FAN    2	/* 打开电扇请求 */
#define OPEN_LAMP   3	/* 打开电灯请求 */
#define CLOSE_FAN   4	/* 关闭电扇请求 */
#define CLOSE_LAMP  5	/* 关闭电灯请求 */
#define SMOKE       6	/* 烟雾信息 */
#define END         9	/* 关闭服务器  */

/* 与传输有关的 */
#define PORT 8000	        /* 端口号 */
#define IP   "192.168.1.193"	

typedef struct video_data       /* 声明图像数据的结构体 */
{
     int            d_size;  /* 数据大小 */
     unsigned char *d_data;  /* 数据缓冲区 */

} VIDEO_DATA;

/* 
 * 此结构为服务器所特有的结构体，为处理方便而设计，客户端没有。
 */

typedef struct server_node
{
     int                sn_serverfd;   /* 服务器套接字 */
     int                sn_clientfd;   /* 客户端套接字 */
     struct sockaddr_in sn_serveraddr; /* 绑定的服务器地址 */

} SERVER_NODE;

/*=========================socket传送数据封装函数========================*/

extern int InitServer( 	/* 此函数用于服务器端，客户端未用到 */
     int                    type, 
     const struct sockaddr *addr, 
     socklen_t              alen, 
     int                    qlen 
     );

#endif	/* LMH_SOCKET_H */
