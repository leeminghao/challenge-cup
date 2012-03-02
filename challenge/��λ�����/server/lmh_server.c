/*
 *============================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_video_server.c
 * 文件摘要：本文件用于采集人脸信息，并发送到客户端。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/29
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *============================================================================
 */

#include "lmh_v4l.h"
#include "lmh_temphum.h"
#include "lmh_socket.h"		/* my header */
#include "lmh_err.h"

#define LAMP 1
#define FAN  0
#define ON   0
#define OFF  1

/*===============================声明一些静态函数================================*/

static void OpenDevice( void );
static void Smoke( void );
static void Video( V4L_DEVICE *vd );
static void Wait( SERVER_NODE *node );
static void Server( void );

/* 全局变量定义 */
SERVER_NODE g_serverNode;
V4L_DEVICE  g_vd;
int         g_fanLampFd;
int         g_tempHumFd;
int         g_ledBuzzerFd;
int         g_smokeFd;

/* 打开设备 */
static void
OpenDevice( void )
{
     int err;

     /* 打开摄像头 */
     memset( &g_vd, 0, sizeof (V4L_DEVICE) );
     err = InitV4LDevice( /* 初始化视频设备 */
	  &g_vd,
	  DEFAULT_DEVICE,
	  PIC_WIDTH,
	  PIC_HEIGHT,
	  PIC_FORMAT,
	  GRAB_METHOD 
	  );
     if ( err < 0 )
     {
	  ErrSys( "InitV4LDevice error" );
     }

     /* 打开电扇，风扇控制文件描述符 */
     system( "mknod /dev/dddf c 240 0" ); /* 加载电灯电扇驱动的设备号 */
     g_fanLampFd = open( "/dev/dddf", 0 );  /* 打开设备文件 */
     if ( g_fanLampFd < 0 )
     {
	  ErrSys( "open g_fanLampFd error" );
     }

     /* 打开报警器 */
     system( "mknod /dev/gpio c 234 0" );
     g_ledBuzzerFd = open( "/dev/gpio", O_RDWR, 0666 );
     if ( g_ledBuzzerFd < 0 )
     {
	  ErrSys( "open g_ledBuzzerFd error" );
     }

     /* 打开温湿度传感器 */
     system( "mknod /dev/sht11 c 238 0" );
     g_tempHumFd = open( "/dev/sht11", O_RDWR, 0666 );
     if ( g_tempHumFd < 0 )
     {
	  ErrSys( "open g_tempHumFd error" );
     }

     /* 打开烟雾传感器 */
     /*system( "mknod /dev/smog c 231 0" );
     g_smokeFd = open( "/dev/smog", O_RDWR, 0666 );
     if ( g_smokeFd < 0 )
     {
	  ErrSys( " open g_smokeFd error" );
	  }*/
}

/* 处理视频数据请求 */
static void
Video( V4L_DEVICE *vd )
{
     VIDEO_DATA  data;
     int         have   = 0;
     int         nwrite = 0;
     int         err;

     /* 采集人脸信息 */
     memset( &data, 0, sizeof (VIDEO_DATA) );
     data.d_size = V4LGrab( vd );
     printf( "size = %d\n", data.d_size );
     data.d_data = (unsigned char *)realloc(
	  data.d_data,
	  data.d_size * sizeof (unsigned char)
	  );
     memset( data.d_data, 0, data.d_size * sizeof (unsigned char) );
     if ( data.d_data == NULL )
     {
	  ErrSys( "Server/realloc error" );
     }
     memcpy( data.d_data, vd->vd_frame, data.d_size );

     /* 发送人脸信息 */
     err = send( 
	  g_serverNode.sn_clientfd, 
	  &data.d_size, 
	  sizeof (data.d_size),
	  MSG_DONTWAIT
	  );
     if ( err < 0 )
     {
	  ErrSys( "Server/write video size error" );
     }
     for ( have = 0; have < data.d_size; have += nwrite )
     {
	  nwrite = send( 
	       g_serverNode.sn_clientfd, 
	       &data.d_data[have], 
	       PER_MAX,
	       MSG_DONTWAIT 
	       );
	  if ( nwrite < 0 )
	  {
	       ErrSys( "Server/write video data error" );
	  }
     }

     free( data.d_data );
     data.d_data = NULL;
}

/* 
 * 发送烟雾信息。
 */
static void
Smoke( void )
{
     int value = 0;
     int ret;

     ret = read( g_smokeFd, &value, sizeof (value) );
     if ( ret < 0 )
     {
	  ErrRet( "read smoke error" );
	  return;
     }

     ret = send(
	  g_serverNode.sn_clientfd,
	  &value,
	  sizeof (value),
	  MSG_DONTWAIT
	  );
     if ( ret < 0 )
     {
	  ErrSys( "send smoke error" );
     }
}

/* 
 * 服务器，用于分别处理客户端的各种请求。
 */
static void
Server( void )
{
     int err;
     int type;

     Wait( &g_serverNode );		/* 等待连接请求 */

     OpenDevice();

     while ( 1 )
     {
	  err = recv( 		/* 读取客户端请求的类型 */
	       g_serverNode.sn_clientfd, 
	       &type, 
	       sizeof (type),
	       MSG_WAITALL
	       );
	  if ( err < 0 )
	  {
	       ErrSys( "Server/read data type error" );
	  }
	  if ( type == END )	/* 结束服务器进程 */
	  {
	       break;
	  }

	  switch ( type )
	  {
	  case VIDEO:		/* 请求视频数据 */
	       printf( "type, video\n" );
	       Video( &g_vd );	/* 采集发送视频数据 */
	       break;

	  case TEMP_HUM:     /* 请求温湿度度数据 */
	       printf( "type, temperature\n" );
	       TempHum();
	       break;

	  case OPEN_FAN:	/* 打开电扇 */
	       ioctl( g_fanLampFd, ON, FAN );
	       break;

	  case OPEN_LAMP:	/* 打开电灯 */
	       ioctl( g_fanLampFd, ON, LAMP );
	       break;
	       
	  case CLOSE_FAN:	/* 关闭电扇 */
	       ioctl( g_fanLampFd, OFF, FAN );
	       break;

	  case CLOSE_LAMP:	/* 关闭电灯 */
	       ioctl( g_fanLampFd, OFF, LAMP );
	       break;

	  case SMOKE:		/* 发送烟雾信息 */
	       Smoke();
	       break;

	  default:
	       printf( "Error type\n" );
	       break;
	  }
     }

     printf( "Now, Close the server\n" );
     close( g_fanLampFd );
     CloseV4L( &g_vd );
}

/* 
 * 为服务器配置地址，并等待客户端接入。
 */
static void
Wait( SERVER_NODE *node )
{
     /* 配置服务器地址 */
     memset( &node->sn_serveraddr, 0, sizeof (struct sockaddr_in) );
     node->sn_serveraddr.sin_family      = AF_INET;
     node->sn_serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );
     node->sn_serveraddr.sin_port        = htons( PORT );

     node->sn_serverfd = InitServer( /* 创建一个套接字并绑定监听地址 */
	  SOCK_STREAM,
	  (struct sockaddr *)&node->sn_serveraddr,
	  sizeof (struct sockaddr_in),
	  MAX_LISTEN
	  );

     /* 接收到客户端的连接 */
     node->sn_clientfd = accept( node->sn_serverfd, NULL, NULL );
     if ( node->sn_clientfd < 0 )
     {
	  ErrSys( "Wait/accept error" );
     }
}

int
main( void )
{
     Server();

     return EXIT_SUCCESS;
}
