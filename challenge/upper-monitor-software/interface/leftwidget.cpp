/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：leftwidget.cpp
 * 文件摘要：本头文件用于声明系统用于显示检测到人脸视频和拍照,录制视频的窗口部件类
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
#include "v4lcapturewidget.h"
#include "leftwidget.h"
#include <QtGui>

LeftWidget::LeftWidget( QWidget *parent /* = NULL */ )
    : QWidget( parent )
{
    leftGroupBox     = new QGroupBox( tr( "快速面部检测窗口" ) );
    v4lCaptureWidget = new V4LCaptureWidget;
    photoVideoWidget = new PhotoVideoWidget;

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    QSplitter   *mainSplitter   = new QSplitter( Qt::Vertical );

    mainSplitter->addWidget( v4lCaptureWidget );
    mainSplitter->addWidget( photoVideoWidget );
    groupBoxLayout->addWidget( mainSplitter );
    leftGroupBox->setLayout( groupBoxLayout );
    leftGroupBox->setStyleSheet(
        "QGroupBox{ margin: 5px 0px 0px 5px; padding: 10px; border: 1px solid blue; border-radius: 5px }"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget( leftGroupBox );

    setLayout( mainLayout );
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
}

LeftWidget::~LeftWidget( void )
{
}

// 获取拍照窗口部件。
PhotoVideoWidget *
LeftWidget::getPhotoVideoWidget( void )
{
    return photoVideoWidget;
}

// 获取视频窗口部件
V4LCaptureWidget *
LeftWidget::getV4LCaptureWidget( void )
{
    return v4lCaptureWidget;
}
