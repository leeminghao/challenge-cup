/*
 *============================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_socket.c
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

#include "lmh_socket.h"
#include "lmh_err.h"

/* 
 * 服务器初始化套接字端点
 */
int
InitServer( 
     int                    type, 
     const struct sockaddr *addr, 
     socklen_t              alen, 
     int                    qlen 
     )
{
     int fd;

     if ( (fd = socket( addr->sa_family, type, 0)) < 0 )
     {
	  ErrSys( "InitServer/socket error" );
     }
     if ( bind( fd, addr, alen ) < 0 )
     {
	  ErrSys( "InitServer/bind error" );
     }
     if ( type == SOCK_STREAM || type == SOCK_SEQPACKET )
     {
	  if ( listen( fd, qlen ) < 0 )
	  {
	       ErrSys( "InitServer/listen error" );
	  }
     }

     return fd;
}
