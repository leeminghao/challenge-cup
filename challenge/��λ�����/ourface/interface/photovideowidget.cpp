/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：photovideowidget.cpp
 * 文件摘要：本头文件用于实现系统用于拍照和录制视频的窗口部件类
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/04
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *==================================================================================
 */

#include "photovideowidget.h"
#include "capture/client.h"

#include <QtGui>

#include <cv.h>
#include <highgui.h>

// 全局变量的声明
extern Client   *g_client;            // 客户端
extern bool      g_isStartRecord;     // 判断是否开始录制视频
extern QString   g_recordVideoName;   // 录制视频名称
extern bool      g_hasConnectServer;  // 判断是否连接到服务器
extern IplImage *g_findImage;

QImage *g_photoImage;

PhotoVideoWidget::PhotoVideoWidget( QWidget *parent /* = NULL */ )
    : QWidget( parent )
{
    g_photoImage    = NULL;

    photoPushButton = new QPushButton( tr( "拍照" ), this );
    videoPushButton = new QPushButton( tr( "录像" ), this );
    photoToolBar    = new QToolBar;
    photoToolBar->setStyleSheet(
       "border: 4px solid blue; border-radius: 5px"
       );

    for ( int i = 0; i < PHOTO_NUM; i++ )
    {
        photoLabel[i] = new QLabel;
        photoLabel[i]->setFixedSize( 163, 140 );
        photoLabel[i]->setStyleSheet( "border: 2px solid green; border-radius: 5px" );
        photoToolBar->addSeparator();
        photoToolBar->addWidget( photoLabel[i] );
    }
    photoToolBar->addSeparator();

    connect( photoPushButton, SIGNAL( clicked() ), this, SLOT( takePhoto() ) );
    connect( videoPushButton, SIGNAL( clicked() ), this, SLOT( recordVideo() ) );

    QHBoxLayout *topLayout = new QHBoxLayout;

    topLayout->addWidget( photoPushButton );
    topLayout->addWidget( videoPushButton );

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addLayout( topLayout );
    mainLayout->addWidget( photoToolBar );

    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    setLayout( mainLayout );
}

PhotoVideoWidget::~PhotoVideoWidget( void )
{
}

void
PhotoVideoWidget::paintEvent( QPaintEvent * /* event */ )
{
}

// 声明进行拍照的槽。
void
PhotoVideoWidget::takePhoto( void )
{
    if ( g_hasConnectServer )
    {
        QDateTime time     = QDateTime::currentDateTime();
        QString   fileName = time.toString( "yyyy.MM.dd_hh:mm:ss" );

        fileName.append( ".jpg" );
        g_photoImage->save(
            tr( "./database/faces/images/%1" ).arg( fileName )
            );
        photoLabel[count % 3]->setPixmap( QPixmap::fromImage( g_photoImage->scaled( 163, 140 ) ) );
        count++;
    }
}

// 声明进行录像的槽。
void
PhotoVideoWidget::recordVideo( void )
{
}
