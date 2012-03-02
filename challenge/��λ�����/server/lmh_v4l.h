/*
 *============================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_v4l.h
 * 文件摘要：本头文件用于声明系统使用video4linux采集视频的接口,用于fl2440开发板
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/27
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *============================================================================
 */

#ifndef LMH_V4L_H
#define LMH_V4L_H

/* ISO c headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Linux c headers */
#include <linux/types.h>
#include <linux/videodev.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#define VIDEO_PALETTE_JPEG 21
#define BRIDGE_ZC3XX        0
#define MAX_BRIDGE          2
#define JPEG                0
#define UNOW                1

/* 默认摄像头设备 */
//#define DEFAULT_DEVICE "/dev/v4l/video0" /* FL2440 */
//#define DEFAULT_DEVICE "/dev/video1" /* PC */
#define DEFAULT_DEVICE "/dev/video0" /* 博创 */

#define PIC_WIDTH   240		        /* 图片宽度 */
#define PIC_HEIGHT  289		        /* 图片高度 */

#define PIC_FORMAT  VIDEO_PALETTE_JPEG	/* 图片格式 */

#define GRAB_METHOD 0		/* 使用read读取一帧图片数据 */

typedef struct bridge_list
{
     int         bl_num;
     const char *bl_name;

} BRIDGE_LIST;

typedef struct v4l_device	/* 自定义v4l设备 */
{
     int                     vd_fd;         /* 设备号 */
     char                   *vd_device;     /* 设备名称 */
     struct video_mmap       vd_mmap;       /* 用于mmap */
     int                     vd_mmapsize;
     struct video_capability vd_capability; /* 包含设备的基本信息 */
     struct video_picture    vd_picture;    /* 设备采集图像的各种属性 */
     struct video_mbuf       vd_mbuf;	    /* 用于mmap惊醒映射的帧信息 */
     struct video_channel    vd_channel;
     int                     vd_cameratype; /* 摄像头类型 */
     char                   *vd_cameraname; /* 摄像头名称 */
     char                    vd_bridge[9];
     int                     vd_palette;
     int                     vd_grabmethod; /* 采集图像方式 */
     unsigned char          *vd_framebuf;
     unsigned char          *vd_frame;
     int                     vd_framesize;
     int                     vd_bpp;
     int                     vd_width;
     int                     vd_height;
     int                     vd_format;

} V4L_DEVICE;

/*===============================采集视频函数==============================*/

extern int InitV4LDevice( 
     V4L_DEVICE *vd, 
     char       *device, 
     int         width, 
     int         height, 
     int         format, 
     int         grabmethod 
     );
	
extern int V4LGrab( V4L_DEVICE *vd );

extern int CloseV4L( V4L_DEVICE *vd );

extern int GetJpegSize( unsigned char *buf, int insize );

#endif	/* LMH_V4L_H */
