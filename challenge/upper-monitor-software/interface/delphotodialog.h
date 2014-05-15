/*
 *===================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：delphotodialog.h
 * 文件摘要：本文件用于声明删除照片的对话框类, 为了方便用户删除不需要的照片。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/07
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *===================================================================================================
 */

#ifndef DELPHOTODIALOG_H
#define DELPHOTODIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class DelPhotoDialog : public QDialog
{
    Q_OBJECT

public:
     DelPhotoDialog( QWidget *parent = NULL );
    ~DelPhotoDialog( void );

private slots:
    void addPhotos( void );
    void delPhotos( void );

private:
    QString      strippedName( const QString &fullFileName );

    QStringList  files;
    QLabel      *photosLabel;
    QLineEdit   *photosLineEdit;
    QPushButton *browsePushButton;
    QPushButton *delPushButton;
    QPushButton *cancelPushButton;
};

#endif // DELPHOTODIALOG_H
