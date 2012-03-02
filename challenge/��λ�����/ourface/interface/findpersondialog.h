/*
 *===================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：findpersondialog.h
 * 文件摘要：本文件用于声明查找人的信息的对话框类, 为了方便用户使用某人相片查找某人。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/03
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *===================================================================================================
 */

#ifndef FINDPERSONDIALOG_H
#define FINDPERSONDIALOG_H

#include "recognize/facerecognize.h"

#include <QDialog>

// OpenCV库头文件
#include <cv.h>
#include <highgui.h>

class QLabel;
class QLineEdit;
class QPushButton;

class FindPersonDialog : public QDialog
{
    Q_OBJECT

public:
     FindPersonDialog( QWidget *parent = NULL );
    ~FindPersonDialog( void );

protected:
    void closeEvent( QCloseEvent *event );

private slots:
    void addFace( void );
    void recognize( void );

private:
    QString      strippedName( const QString &fullFileName );

    QString      fileName;

    QLabel      *faceLabel;
    QLineEdit   *faceLineEdit;
    QPushButton *browsePushButton;
    QPushButton *findPushButton;
    QPushButton *cancelPushButton;

    IplImage    *findImage;
};

#endif // FINDPERSONDIALOG_H
