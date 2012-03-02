/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：leftwidget.h
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

#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>

class QGroupBox;
class V4LCaptureWidget;
class PhotoVideoWidget;

class LeftWidget : public QWidget
{
public:
     LeftWidget( QWidget *parent = NULL );
    ~LeftWidget( void );

    PhotoVideoWidget *getPhotoVideoWidget( void );
    V4LCaptureWidget *getV4LCaptureWidget( void );

private:
    QGroupBox        *leftGroupBox;

    V4LCaptureWidget *v4lCaptureWidget;
    PhotoVideoWidget *photoVideoWidget;
};

#endif // LEFTWIDGET_H
