/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：personinfowidget.h
 * 文件摘要：本头文件用于声明系统用于显示人的信息的类, 文系统现在采用一个简单的文本剪贴板用于显示，
 *         为了方便以后升级，故声明此类。
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

#ifndef PERSONINFOWIDGET_H
#define PERSONINFOWIDGET_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QGroupBox;

class PersonInfoWidget: public QWidget
{
    Q_PROPERTY( QString text READ text WRITE setText )

public:
     PersonInfoWidget( QWidget *parent = NULL );
    ~PersonInfoWidget( void );

    void setFace( const QImage &face );
    void setName( const QString &name );
    void setNum( int num );

private:
    QGroupBox *personGroupBox;
    QLabel    *faceLabel;
    QLabel    *nameLabel;
    QLabel    *numLabel;
    QLineEdit *nameLineEdit;
    QLineEdit *numLineEdit;
};

#endif // PERSONINFOWIDGET_H
