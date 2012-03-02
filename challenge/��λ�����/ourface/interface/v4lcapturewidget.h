/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：v4lcapturewidget.h
 * 文件摘要：本头文件用于声明系统使用video4linux采集视频显示的接口
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/21
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *==================================================================================
 */

#ifndef V4LCAPTUREWIDGET_H
#define V4LCAPTUREWIDGET_H

#include "capture/client.h"
#include <QWidget>
#include <QImage>
#include <QString>
#include <QtGui>
#include <cv.h>    // OpenCV库文件
#include <highgui.h>

class QTimer;
class FaceDetect;

class V4LCaptureWidget : public QWidget
{
    Q_OBJECT

public:
     V4LCaptureWidget( QWidget *parent = NULL );
    ~V4LCaptureWidget( void );

    void recognize( void );

protected:
    void paintEvent( QPaintEvent *event );

private slots:
    void nextFrame( void );

private:
    FaceDetect    *vcw_detect;

    IplImage      *vcw_iplImg;
    IplImage      *vcw_findIplImg;

    QImage        *vcw_findImg;
    QImage        *vcw_qImg;
    QImage         vcw_image;
    QTimer        *vcw_timer;
    unsigned char *vcw_picture;    // 指向存储图像地址的指针
};

#endif // V4LCAPTUREWIDGET_H
