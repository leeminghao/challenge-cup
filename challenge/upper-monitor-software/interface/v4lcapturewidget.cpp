/*
 *==================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：v4lcapturewidget.cpp
 * 文件摘要：本文件用于实现系统使用video4linux采集视频显示的接口
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/21
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *==================================================================================================
 */

#include "v4lcapturewidget.h"
#include "personinfowidget.h"
#include "capture/client.h"
#include "detect/facedetect.h"
#include "recognize/facerecognize.h"

#include <QtGui>

// 全局变量声明
extern bool              g_hasConnectServer;   // 判断是否连接到服务器
extern bool              g_hasDetectFace;      // 判断是否检测到人脸
extern bool              g_isStartRecord;      // 是否开始录制视频。
extern QString           g_recordVideoName;    // 录制视频的保存的文件名
extern PersonInfoWidget *g_personInfoWidget;   // 显示人脸信息
extern FaceRecognize    *g_faceRecognize;      // 人脸识别算法
extern Client           *g_client;             // 客户端

// 全局变量定义
IplImage *g_findImage = NULL;

V4LCaptureWidget::V4LCaptureWidget( QWidget *parent )
    : QWidget( parent )
{
    vcw_picture = NULL;
    vcw_detect  = new FaceDetect;
    vcw_iplImg  = cvCreateImage( cvSize( PIC_WIDTH, PIC_HEIGHT ), IPL_DEPTH_8U, 3 );
    vcw_qImg    = new QImage( PIC_WIDTH, PIC_HEIGHT, QImage::Format_RGB888 );
    vcw_timer   = new QTimer( this );

    vcw_timer->setInterval( 100 );
    connect( vcw_timer, SIGNAL( timeout() ), SLOT( nextFrame() ) );
    vcw_timer->start();
}

V4LCaptureWidget::~V4LCaptureWidget( void )
{
    cvReleaseImage( &vcw_iplImg );
    delete vcw_detect;
}

void
V4LCaptureWidget::paintEvent( QPaintEvent * /* event */ )
{
    QPainter painter( this );

    painter.drawImage(
        QPoint( 98, 0 ), QImage( ":/video/window.jpg" ).scaled( 370, 310 )
        );
    painter.drawImage( QPoint( 123, 20 ), vcw_image );
}

void
V4LCaptureWidget::nextFrame( void )
{
    if ( g_hasConnectServer )
    {
        vcw_picture = g_client->getFrame();
        if ( vcw_picture != NULL )
        {
            vcw_qImg->loadFromData( vcw_picture, g_client->getPictureSize(), "JPEG" );
            vcw_image = vcw_qImg->convertToFormat( QImage::Format_RGB888 );
            vcw_iplImg->imageData = (char *)vcw_image.bits();
            if ( g_isStartRecord )    // 为true开始录制视频。
            {
            }
            vcw_detect->DetectAndDraw( vcw_iplImg );
            if ( g_hasDetectFace )    // 如果检测到人脸图像，则进行识别。
            {
                recognize();
            }
            update();
        }
    }
}

// 对检测到的人脸进行识别
void
V4LCaptureWidget::recognize( void )
{
    IplImage *gray = NULL;

    if ( g_findImage != NULL )
    {
        gray = cvCreateImage(
            cvGetSize( g_findImage ),
            IPL_DEPTH_8U,
            1
            );

        cvCvtColor( g_findImage, gray, CV_RGB2GRAY );

        g_faceRecognize->recognize( gray );
    }

    char **names = NULL;
    int    num;

    names = g_faceRecognize->getResultNames();
    num   = g_faceRecognize->getResultNum();

    if ( num == -1 )
    {
        g_personInfoWidget->setFace( QImage( tr( "./interface/images/splash.jpg" ) ) );
        g_personInfoWidget->setName( QString( tr( "非法者" ) ) );
        g_personInfoWidget->setNum( -1 );
    }
    else
    {
        g_personInfoWidget->setFace( QImage( tr( "./database/info/images/%1" ).arg( num ) ) );
        g_personInfoWidget->setName( names[0] );
        g_personInfoWidget->setNum( num );
    }

    if ( g_findImage != NULL )
    {
        cvReleaseImage( &g_findImage );
        g_findImage = NULL;
        cvReleaseImage( &gray );
    }
}
