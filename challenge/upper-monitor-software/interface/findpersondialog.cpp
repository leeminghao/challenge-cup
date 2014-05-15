/*
 *=================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：findpersondialog.cpp
 * 文件摘要：本文件用于实现查找人的信息的对话框类, 为了方便用户使用某人相片查找某人。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/03
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *=================================================================================================
 */

#include "findpersondialog.h"
#include "personinfowidget.h"
#include <QtGui>

extern PersonInfoWidget *g_personInfoWidget;
extern FaceRecognize    *g_faceRecognize;

FindPersonDialog::FindPersonDialog( QWidget *parent /* = NULL */ )
    : QDialog( parent )
{
    findImage   = NULL;

    faceLabel    = new QLabel( tr( "人脸" ), this );
    faceLineEdit = new QLineEdit;
    faceLineEdit->setReadOnly( true );
    faceLabel->setBuddy( faceLineEdit );
    browsePushButton = new QPushButton( tr( "浏览..." ) );
    findPushButton   = new QPushButton( tr( "识别" ) );
    cancelPushButton = new QPushButton( tr( "取消" ) );

    connect( browsePushButton, SIGNAL( clicked() ), this, SLOT( addFace() ) );
    connect( findPushButton, SIGNAL( clicked() ), this, SLOT( recognize() ) );
    connect( cancelPushButton, SIGNAL( clicked() ), this, SLOT( close() ) );

    QHBoxLayout *topLayout = new QHBoxLayout;

    topLayout->addWidget( faceLabel );
    topLayout->addWidget( faceLineEdit );
    topLayout->addWidget( browsePushButton );

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    bottomLayout->addStretch();
    bottomLayout->addWidget( findPushButton );
    bottomLayout->addWidget( cancelPushButton );

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addLayout( topLayout );
    mainLayout->addLayout( bottomLayout );

    setLayout( mainLayout );
    setWindowTitle( tr( "识别人脸" ) );
}

FindPersonDialog::~FindPersonDialog( void )
{
}

void
FindPersonDialog::closeEvent( QCloseEvent * /* event */ )
{
}

// 添加想要识别的人脸图像。
void
FindPersonDialog::addFace( void )
{
    fileName = QFileDialog::getOpenFileName(
        this,
        tr( "打开人脸图像" ),
        "./database/faces/images/",
        tr( "人脸图像 (*.jpg *.bmp *.png)" )
        );

    if ( fileName.isEmpty() )
    {
        return;
    }

    findImage = cvLoadImage(
        (char *)fileName.toStdString().c_str(),
        CV_LOAD_IMAGE_GRAYSCALE
        );
    if ( findImage == NULL )
    {
        int r = QMessageBox::warning(
            this,
            tr( "警告" ),
            tr( "载入待识别人脸失败！请您重新载入" ),
            QMessageBox::Ok
            );

        if ( r == QMessageBox::Ok )
        {
            return;
        }
    }
    faceLineEdit->setText( strippedName( fileName ) );
}

// 使用添加的人脸图像查询人员信息。
void
FindPersonDialog::recognize( void )
{
    if ( faceLineEdit->text().isEmpty() )
    {
        int r = QMessageBox::warning(
            this,
            tr( "警告" ),
            tr( "您尚未添加人脸图像,请您添加人脸图像" ),
            QMessageBox::Ok
            );
        if ( r == QMessageBox::Ok )
        {
            return;
        }
    }

    g_faceRecognize->recognize( findImage );

    char **names = NULL;
    int    num;

    names = g_faceRecognize->getResultNames();
    num   = g_faceRecognize->getResultNum();

    if ( num == -1 )
    {
        g_personInfoWidget->setFace( QImage( tr( "./interface/images/splash.jpg" ) ) );
        g_personInfoWidget->setName( QString( tr( "非法者" ) ) );
        g_personInfoWidget->setNum( -1 );
    }
    else
    {
        g_personInfoWidget->setFace( QImage( tr( "./database/info/images/%1" ).arg( num ) ) );
        g_personInfoWidget->setName( names[0] );
        g_personInfoWidget->setNum( num );
    }
    faceLineEdit->clear();
}

QString
FindPersonDialog::strippedName( const QString &fullFileName )
{
    return QFileInfo( fullFileName ).fileName();
}
