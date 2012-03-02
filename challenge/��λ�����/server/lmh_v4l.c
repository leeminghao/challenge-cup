/*
 *============================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lmh_v4l.c
 * 文件摘要：本文件用于实现系统使用video4linux采集人脸的接口,用于fl2440开发板
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

#include "lmh_v4l.h"
#include "lmh_err.h"

/*=================================静态函数声明=================================*/

static int ConvertFrame(
     unsigned char *dst, 
     unsigned char *src,
     int            width,
     int            height,
     int            format,
     int            size
     );
static int InitV4L( V4L_DEVICE *vd );
static int IsSpcaChip( const char *bridgename );
static int GetStreamID( const char *bridgename );
static int GetDepth( int format );
static int GetVideoPicture( V4L_DEVICE *vd );
static int SetVideoPicture( V4L_DEVICE *vd );

static BRIDGE_LIST g_list[] =
{
//     { BRIDGE_ZC3XX, "zc3xx"   }, /* pc     */
     { BRIDGE_ZC3XX, "ZC301-2" } /* FL2440 */
};

/* 
 * 初始化V4L结构, 成功：返回0，失败：返回-1
 */
int
InitV4LDevice( 
     V4L_DEVICE *vd,    
     char       *device, 
     int         width,
     int         height, 
     int         format, 
     int         grabmethod 
     )
{
     int ret = -1;

     if ( vd == NULL || device == NULL )
     {
	  ErrMsg( "InitV4LDevice error: vd == NULL || device == NULL" );
	  return -1;
     }
     if ( width == 0 || height == 0 )
     {
	  ErrMsg( "InitV4LDevice error: width == 0 || height == 0" );
	  return -1;
     }
     if ( grabmethod < 0 || grabmethod > 1 )
     {
	  grabmethod = 1;	/* read by default */
     }

     vd->vd_device     = NULL;
     vd->vd_cameraname = NULL;
     vd->vd_device     = (char *)realloc( vd->vd_device, 32 );
     if ( vd->vd_device == NULL )
     {
	  ErrSys( "InitV4LDevice realloc vd->vd_device errro" );
     }
     vd->vd_cameraname = (char *)realloc( vd->vd_cameraname, 32 );
     if ( vd->vd_cameraname == NULL )
     {
	  ErrSys( "InitV4LDevice realloc vd->vd_cameraname error" );
     }
     snprintf( vd->vd_device, 32, "%s", device );
     memset( vd->vd_cameraname, 0, sizeof (vd->vd_cameraname) );
     memset( vd->vd_bridge, 0, sizeof (vd->vd_bridge) );
     vd->vd_width      = width;
     vd->vd_height     = height;
     vd->vd_format     = format;
     vd->vd_bpp        = GetDepth( vd->vd_format );
     vd->vd_grabmethod = grabmethod;
     vd->vd_framebuf   = NULL;
     ret               = InitV4L( vd );
     vd->vd_frame      = NULL;
     vd->vd_frame      = (unsigned char *)realloc( 
	  vd->vd_frame, (size_t)vd->vd_framesize 
	  );

     return ret;
}

/* 
 * 关闭摄像头，成功：返回0
 */
int
CloseV4L( V4L_DEVICE *vd )
{
     if ( vd->vd_grabmethod )
     {
	  munmap( vd->vd_framebuf, vd->vd_mmapsize );
     }
     else
     {
	  free( vd->vd_framebuf );
	  vd->vd_framebuf = NULL;
     }

     if ( close( vd->vd_fd ) < 0 )
     {
	  ErrRet( "CloseV4L error: close error" );
	  return -1;
     }

     /* 释放分配的内存 */
     if ( vd->vd_device != NULL )
     {
	  free( vd->vd_device );
	  vd->vd_device = NULL;
     }
     if ( vd->vd_cameraname != NULL )
     {
	  free( vd->vd_cameraname );
	  vd->vd_cameraname = NULL;
     }
     if ( vd->vd_frame != NULL )
     {
	  free( vd->vd_frame );
	  vd->vd_frame = NULL;
     }

     return 0;
}

int
GetJpegSize( unsigned char *buf, int insize )
{
     int i;

     for ( i = 1024; i < insize; i++ )
     {
	  if ( (buf[i] == 0xff && (buf[i+1] == 0xD9)) )
	  {
	       return i + 2;
	  }
     }

     return -1;
}


/* 
 * 使用V4L抓取一帧图像，成功：返回图像大小，失败：返回-1
 */
int
V4LGrab( V4L_DEVICE *vd )
{
     int        len;
     int        size;
     int        jpegsize = -1;

     if ( vd->vd_grabmethod )
     {
	  vd->vd_mmap.height = vd->vd_height;
	  vd->vd_mmap.width  = vd->vd_width;
	  vd->vd_mmap.format = vd->vd_format;

	  if ( ioctl( vd->vd_fd, VIDIOCMCAPTURE, &(vd->vd_mmap) ) < 0 )
	  {
	       ErrSys( "V4LGrab error/VIDIOCMCAPTURE" );
	  }
	  if ( ioctl( vd->vd_fd, VIDIOCSYNC, &(vd->vd_mmap.frame) ) < 0 )
	  {
	       ErrSys( "V4LGrab error/VIDIOCSYNC" );
	  }

	  jpegsize = ConvertFrame( 
	       vd->vd_frame,
	       vd->vd_framebuf + vd->vd_mbuf.offsets[vd->vd_mmap.frame],
	       vd->vd_width,
	       vd->vd_height,
	       vd->vd_format,
	       vd->vd_framesize
	       );

	  return jpegsize;
     }
     else
     {
	  size = vd->vd_framesize;
	  len  = read( vd->vd_fd, vd->vd_framebuf, size );
	  if ( len < 0 )
	  {
	       ErrSys( "V4LGrab read error" );
	  }
	  jpegsize = ConvertFrame(
	       vd->vd_frame,
	       vd->vd_framebuf,
	       vd->vd_width,
	       vd->vd_height,
	       vd->vd_format,
	       vd->vd_framesize
	       );

	  return jpegsize;
     }

     return -1;
}

/*================================以下是静态函数定义================================*/

/* 
 * 摄像头采集图像之前的初始化工作，成功：返回0，失败：终止程序.
 */
static int
InitV4L( V4L_DEVICE *vd )
{
     int ret;

     if ( (vd->vd_fd = open( vd->vd_device, O_RDWR )) < 0 )
     {
	  ErrSys( "InitV4L open error" );
     }
     if ( ioctl( vd->vd_fd, VIDIOCGCAP, &vd->vd_capability ) < 0 )
     {
	  ErrSys( "InitV4L get video capture error" );
     }
     snprintf( vd->vd_cameraname, 32, "%s", vd->vd_capability.name );

     ret = GetVideoPicture( vd );

     if ( ioctl( vd->vd_fd, VIDIOCGCHAN, &vd->vd_channel ) < 0 )
     {
	  vd->vd_cameratype = UNOW;
     }
     else
     {
	  if ( vd->vd_channel.name != NULL )
	  {
	       snprintf( vd->vd_bridge, 9, "%s", vd->vd_channel.name );
	       vd->vd_cameratype = GetStreamID( vd->vd_bridge );
	  }
	  else
	  {
	       vd->vd_cameratype = UNOW;
	  }
     }

     if ( vd->vd_cameratype != JPEG )
     {
	  ErrSys( "Not a JPEG webcam sorry Abort!" );
     }
     vd->vd_picture.palette = vd->vd_format;
     vd->vd_picture.depth   = GetDepth( vd->vd_format );
     vd->vd_framesize       = (vd->vd_width * vd->vd_height >> 2);
     ret                    = SetVideoPicture( vd );
     if ( vd->vd_format != vd->vd_picture.palette
       || vd->vd_bpp    != vd->vd_picture.depth )
     {
	  ErrSys( "couldn't set video palette Abort!" );
     }
     if ( vd->vd_grabmethod )
     {
	  memset( &vd->vd_mbuf, 0, sizeof (vd->vd_mbuf) );
	  if ( ioctl( vd->vd_fd, VIDIOCGMBUF, &vd->vd_mbuf ) < 0 )
	  {
	       ErrSys( "InitV4L get mbuf error" );
	  }
	  vd->vd_framebuf = (unsigned char *)mmap( 
	       0, 
	       vd->vd_mbuf.size,
	       PROT_READ | PROT_WRITE,
	       MAP_SHARED,
	       vd->vd_fd,
	       0
	       );
	  if ( vd->vd_framebuf == MAP_FAILED )
	  {
	       ErrSys( "InitV4L mmap error" );
	  }
	  vd->vd_mmapsize    = vd->vd_mbuf.size;
	  vd->vd_mmap.height = vd->vd_height;
	  vd->vd_mmap.width  = vd->vd_width;
	  vd->vd_mmap.format = vd->vd_format;
	  vd->vd_mmap.frame  = 0;
	  if ( ioctl( vd->vd_fd, VIDIOCMCAPTURE, &vd->vd_mmap ) < 0 )
	  {
	       ErrSys( "V4LInit get capture error" );
	  }
     }
     else
     {
	  vd->vd_framebuf = (unsigned char *)realloc(
	       vd->vd_framebuf,
	       (size_t)vd->vd_framesize
	       );
     }

     return ret;
}

/* 
 * 转换图像格式，成功：返回图像大小，失败：返回-1
 */
static int
ConvertFrame(
     unsigned char *dst,
     unsigned char *src,
     int            width,
     int            height,
     int            format,
     int            size
     )
{
     int jpegsize = -1;

     switch ( format )
     {
     case VIDEO_PALETTE_JPEG:
	  jpegsize = GetJpegSize( src, size );
	  if ( jpegsize < 0 )
	  {
	       jpegsize = -1;
	       break;
	  }
	  memcpy( dst, src, jpegsize );
	  break;

     default:
	  break;
     }

     return jpegsize;
}

/* 
 * 获取图像属性，成功：返回0，失败：终止程序
 */
static int
GetVideoPicture( V4L_DEVICE *vd )
{
     if ( ioctl( vd->vd_fd, VIDIOCGPICT, &vd->vd_picture ) < 0 )
     {
	  ErrSys( "GetVideoPicture error" );
     }

     return 0;
}

/* 
 * 设置图像属性, 成功：返回0, 失败：终止程序
 */
static int
SetVideoPicture( V4L_DEVICE *vd )
{
     if ( ioctl( vd->vd_fd, VIDIOCSPICT, &vd->vd_picture ) < 0 )
     {
	  ErrSys( "SetVideoPicture error" );
     }

     return 0;
}

static int
IsSpcaChip( const char *bridgename )
{
     int i    = -1;
     int find = -1;
     int size =  0;

     for ( i = 0; i < MAX_BRIDGE - 1; i++ )
     {
	  size = strlen( g_list[i].bl_name );
	  if ( strncmp( bridgename, g_list[i].bl_name, size ) == 0 )
	  {
	       find = i;
	       break;
	  }
     }

     return find;
}

static int
GetStreamID( const char *bridgename )
{
     int i     = -1;
     int match = -1;

     if ( (match = IsSpcaChip( bridgename )) < 0 )
     {
	  return match;
     }

     switch ( match )
     {
     case BRIDGE_ZC3XX:
	  i = JPEG;
	  break;
     }

     return i;
}

static int
GetDepth( int format )
{
     int depth;

     switch ( format )
     {
     case VIDEO_PALETTE_JPEG:
	  depth = 8;
	  break;

     default:
	  depth = -1;
	  break;
     }

     return depth;
}
