/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：photovideowidget.h
 * 文件摘要：本头文件用于声明系统用于拍照和录制视频的窗口部件类
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

#ifndef PHOTOVIDEOWIDGET_H
#define PHOTOVIDEOWIDGET_H

#include <QWidget>

#define PHOTO_NUM 3

class QLabel;
class QPushButton;
class QAction;
class QToolBar;

class PhotoVideoWidget : public QWidget
{
   Q_OBJECT

public:
    PhotoVideoWidget( QWidget *parent = NULL );
   ~PhotoVideoWidget( void );

protected:
    void paintEvent( QPaintEvent *event );

public slots:
    void takePhoto( void );
    void recordVideo( void );

private:
    int          count;

    QPushButton *photoPushButton;
    QPushButton *videoPushButton;
    QLabel      *photoLabel[PHOTO_NUM];
    QToolBar    *photoToolBar;
};

#endif // PHOTOVIDEOWIDGET_H
