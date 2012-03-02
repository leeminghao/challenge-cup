/*
 *===================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：addpersondialog.h
 * 文件摘要：本文件用于声明人的信息的对话框类, 为了方便用户添加人信息。
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

#ifndef ADDPERSONDIALOG_H
#define ADDPERSONDIALOG_H

#include "recognize/facerecognize.h"
#include <QDialog>

#define FILE_SIZE 256

class QLabel;
class QLineEdit;
class QPushButton;

class AddPersonDialog : public QDialog
{
    Q_OBJECT

public:
     AddPersonDialog( QWidget *parent = NULL );
    ~AddPersonDialog( void );

private slots:
    void addFace( void );
    void addPerson( void );

private:
    QString strippedName( const QString &fullFileName );

    QLabel      *numLabel;
    QLabel      *nameLabel;
    QLabel      *faceLabel;
    QLineEdit   *numLineEdit;
    QLineEdit   *nameLineEdit;
    QLineEdit   *faceLineEdit;
    QPushButton *browsePushButton;
    QPushButton *okPushButton;
    QPushButton *cancelPushButton;

    char          *name;
    Face          *face;
    Person        *person;
};

#endif // ADDPERSONDIALOG_H

