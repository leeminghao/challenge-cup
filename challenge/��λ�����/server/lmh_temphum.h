/*
 *============================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_temphum.h
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

#ifndef __LMH_TEMPHUM_H
#define __LMH_TEMPHUM_H

#define TEMPERATURE 0
#define HUMIDITY    1
#define LED         1
#define BUZZER      0
#define LB_ON       1
#define LB_OFF      0

extern void  CalcSht11( float *humidity, float *temperature );
extern float CalcDewpoint( float h, float t );
extern void  Delay( int time );
extern void  TempHum( void );

#endif	/* __LMH_TEMPHUM_H */
