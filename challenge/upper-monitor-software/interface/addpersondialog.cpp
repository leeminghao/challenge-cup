/*
 *=================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：addpersondialog.cpp
 * 文件摘要：本文件用于实现增加人的信息的对话框类, 为了方便用户添加人信息。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/01
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *=================================================================================================
 */

#include "addpersondialog.h"
#include <string.h>
#include <QtGui>

extern FaceRecognize *g_faceRecognize;

AddPersonDialog::AddPersonDialog( QWidget *parent /* = NULL */ )
    : QDialog( parent )
{
    numLabel     = new QLabel( tr( "编号(&I)：" ), this );
    nameLabel    = new QLabel( tr( "姓名(&N)：" ), this );
    faceLabel    = new QLabel( tr( "人脸(&F)：" ), this );

    numLineEdit  = new QLineEdit;
    nameLineEdit = new QLineEdit;
    faceLineEdit = new QLineEdit;
    faceLineEdit->setReadOnly( true );
    numLabel->setBuddy( numLineEdit );
    nameLabel->setBuddy( nameLineEdit );
    faceLabel->setBuddy( faceLineEdit );
    browsePushButton = new QPushButton( tr( "浏览..." ) );
    okPushButton     = new QPushButton( tr( "确认" ) );
    cancelPushButton = new QPushButton( tr( "取消" ) );
    cancelPushButton->setDefault( true );

    connect( browsePushButton, SIGNAL( clicked() ), this, SLOT( addFace() ) );
    connect( okPushButton, SIGNAL( clicked() ), this, SLOT( addPerson() ) );
    connect( cancelPushButton, SIGNAL( clicked() ), this, SLOT( close() ) );

    QGridLayout *topLayout = new QGridLayout;

    topLayout->addWidget( numLabel, 0, 0, 1, 2 );
    topLayout->addWidget( numLineEdit, 0, 1, 1, 2 );
    topLayout->addWidget( nameLabel, 1, 0, 1, 2 );
    topLayout->addWidget( nameLineEdit, 1, 1, 1, 2 );
    topLayout->addWidget( faceLabel, 2, 0, 1, 1 );
    topLayout->addWidget( faceLineEdit, 2, 1, 1, 1 );
    topLayout->addWidget( browsePushButton, 2, 2, 1, 1 );

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    bottomLayout->addStretch();
    bottomLayout->addWidget( okPushButton );
    bottomLayout->addWidget( cancelPushButton );

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addLayout( topLayout );
    mainLayout->addLayout( bottomLayout );

    setLayout( mainLayout );
    setModal( true );
    setWindowTitle( tr( "添加人员信息" ) );
}

AddPersonDialog::~AddPersonDialog( void )
{
    delete face;
    delete person;
}

// 添加人的信息的槽
void
AddPersonDialog::addPerson( void )
{
    if ( numLineEdit->text().isEmpty() )
    {
        int r = QMessageBox::warning(
            this,
            tr( "警告" ),
            tr( "您添加的人员没有编号请您重新添加！" ),
            QMessageBox::Ok
            );
        if ( r == QMessageBox::Ok )
        {
            return;
        }
    }
    if ( nameLineEdit->text().isEmpty() )
    {
        int r = QMessageBox::warning(
            this,
            tr( "警告" ),
            tr( "您添加的人员没有姓名请您重新添加!" ),
            QMessageBox::Ok
            );
        if ( r == QMessageBox::Ok )
        {
            return;
        }
    }
    if ( faceLineEdit->text().isEmpty() )
    {
        int r = QMessageBox::warning(
            this,
            tr( "警告" ),
            tr( "您添加的人员没有人脸图像,请您重新添加人员命令" ),
            QMessageBox::Ok
            );
        if ( r == QMessageBox::Ok )
        {
            return;
        }
    }

    int r = QMessageBox::warning(
        this,
        tr( "提示" ),
        tr( "您是否保存人员信息?" ),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

    if ( r == QMessageBox::Yes )
    {
        person = new Person(    // 新增一个人员的信息
            numLineEdit->text().toInt(),
            (char *)nameLineEdit->text().toStdString().c_str(),
            face
            );
        g_faceRecognize->addPersonToList( person );                   // 将新添加的人加入待识别的链表中。
        g_faceRecognize->trainPerson( g_faceRecognize->getPersonList().size() ); // 训练某个人的人脸图像。

        QString hmmFileName;
        QString infoFileName;

        hmmFileName = "./database/faces/hmm_model/" + numLineEdit->text() + ".hmm";

        person->saveFace( (char *)hmmFileName.toStdString().c_str() );

        infoFileName = "./database/info/person.info";

        g_faceRecognize->savePerson( (char *)infoFileName.toStdString().c_str() );
        numLineEdit->clear();
        nameLineEdit->clear();
        faceLineEdit->clear();
    }
    else if ( r == QMessageBox::No )
    {
        return;
    }
    else if ( r == QMessageBox::Cancel )
    {
        return;
    }
}

// 添加人脸图像的槽
void
AddPersonDialog::addFace( void )
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        tr( "添加人脸图像" ),
        "./database/faces/images/",
        tr( "人脸图像 ( *.jpg *.bmp *.png )" )
        );
    QStringList::iterator it = files.begin();
    QString faceText;

    face = new Face;
    while ( it != files.end() )
    {
        faceText += strippedName( *it ) + " ";
        int len = strlen( it->toStdString().c_str() );
        name = new char [len + 1];
        memset( name, 0, len + 1 );
        strncpy( name, (char *)it->toStdString().c_str(), len );
        face->addImageToList( name );
        it++;
    }
    faceLineEdit->setText( faceText );
}

QString
AddPersonDialog::strippedName( const QString &fullFileName )
{
    return QFileInfo( fullFileName ).fileName();
}
