/*
 *============================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_temphum.c
 * 文件摘要：本文件用于采集温湿度，并发送到客户端。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/29
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/05/12
 *============================================================================
 */

#include "lmh_temphum.h"
#include "lmh_err.h"
#include "lmh_socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/ioctl.h>

extern int         g_tempHumFd;
extern int         g_ledBuzzerFd;
extern SERVER_NODE g_serverNode;

void
CalcSht11( float *humidity, float *temperature )
{
     const float c1 = -0.40;
     const float c2 = 0.0405;
     const float c3 = -0.0000028;
     const float t1 = 0.01;
     const float t2 = 0.00008;

     float rh = *humidity;
     float t  = *temperature;
     float rhLin;
     float rhTrue;
     float tc;

     tc     = t * 0.01 - 40;
     rhLin  = c3 * rh * rh + c2 * rh + c1;
     rhTrue = (tc - 25) * (t1 + t2 * rh) + rhLin;
     if ( rhTrue > 100 )
     {
	  rhTrue = 100;
     }
     if ( rhTrue < 0.1 )
     {
	  rhTrue = 0.1;
     }
     *temperature = tc;
     *humidity    = rhTrue;
}

float
CalcDewpoint( float h, float t )
{
     float k;
     float dewpoint;

     k        = (log10( h ) - 2) / 0.4343 + (17.62 * t) / (243.12 + t);
     dewpoint = 243.12 * k / (17.62 - k);

     return dewpoint;
}

void
Delay( int time )
{
     int i;

     for ( i = 0; i < time * 1000; i++ )
	  ;
}

void
TempHum( void )
{
     unsigned int valuet  = 0;
     unsigned int valueh  = 0;
     float        fvaluet = 0.0;
     float        fvalueh = 0.0;
     int          ret;

     ioctl( g_tempHumFd, 0 );	/* 开始检测温度 */
     ret = read( g_tempHumFd, &valuet, sizeof (valuet) );
     if ( ret < 0 )
     {
	  ErrRet( "read temperature error" );
	  return;
     }
     valuet  = valuet & 0x3fff;
     fvaluet = (float)valuet;

     ioctl( g_tempHumFd, 1 );	/* 开始检测湿度 */
     ret = read( g_tempHumFd, &valueh, sizeof (valueh) );
     if ( ret < 0 )
     {
	  ErrRet( "read humidtiy error" );
	  return;
     }
     valueh  = valueh & 0xfff;
     fvalueh = (float)valueh;
     CalcSht11( &fvalueh, &fvaluet );

     if ( fvaluet > 30 )
     {
	  ioctl( g_ledBuzzerFd, LB_ON, LED );
	  ioctl( g_ledBuzzerFd, LB_ON, BUZZER );
     }
     else
     {
	  ioctl( g_ledBuzzerFd, LB_OFF, LED );
	  ioctl( g_ledBuzzerFd, LB_OFF, BUZZER );
     }

     /* 传输到客户端 */
     ret = send( 			/* 传输温度信息 */
	  g_serverNode.sn_clientfd,
	  &fvaluet,
	  sizeof (fvaluet),
	  MSG_DONTWAIT
	  );
     if ( ret < 0 )
     {
	  ErrSys( "send temperature error" );
     }

     ret = send(		/* 传输湿度信息 */
	  g_serverNode.sn_clientfd,
	  &fvalueh,
	  sizeof (fvalueh),
	  MSG_DONTWAIT
	  );
     if ( ret < 0 )
     {
	  ErrSys( "send humidtiy error" );
     }
}
