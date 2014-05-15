/*
 *==============================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_err.h
 * 文件摘要：本头文件用于声明系统出错处理函数
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/24
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *==============================================================================
 */

#ifndef LMH_ERR_H
#define LMH_ERR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#define MAXLINE      4096

/*==================================出错处理函数================================*/

extern void ErrRet( const char *fmt, ... );

extern void ErrSys( const char *fmt, ... );

extern void ErrMsg( const char *fmt, ... );

#endif	/* LMH_ERR_H */
