/*
 *============================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_err.c
 * 文件摘要：本头文件用于实现系统出错处理函数
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

#include "lmh_err.h"

static void			/* 声明静态函数 */
ErrDoit( 
     int         errnoflag, 
     int         error, 
     const char *fmt, 
     va_list     ap
     );

/* 
 * 出错处理函数，当调用linux下系统调用失败时使用, 目的是使程序终止.
 */
void
ErrSys( const char *fmt, ... )
{
     va_list ap;

     va_start( ap, fmt );
     ErrDoit( 1, errno, fmt, ap );
     va_end( ap );
     exit( EXIT_FAILURE );
}

/* 
 * 出错处理函数，打印出错信息，并返回到原程序。
 */
void
ErrRet( const char *fmt, ... )
{
     va_list ap;

     va_start( ap, fmt );
     ErrDoit( 1, errno, fmt, ap );
     va_end( ap );
}

/* 
 * 出错处理函数，打印出错信息，并返回到源程序。
 */
void
ErrMsg( const char *fmt, ... )
{
     va_list ap;

     va_start( ap, fmt );
     ErrDoit( 0, 0, fmt, ap );
     va_end( ap );
}

static void
ErrDoit( 
     int         errnoflag, 
     int         error, 
     const char *fmt, 
     va_list     ap
     )
{
     char buf[MAXLINE];

     vsnprintf( buf, MAXLINE, fmt, ap );
     if ( errnoflag )
     {
	  snprintf( 
	       buf + strlen( buf ), 
	       MAXLINE - strlen( buf ),
	       ": %s",
	       strerror( error )
	       );
     }
     strcat( buf, "\n" );
     fflush( stdout );
     fputs( buf, stderr );
     fflush( NULL );
}
