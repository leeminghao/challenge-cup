/*
 *===================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：delphotodialog.cpp
 * 文件摘要：本文件用于实现删除照片的对话框类, 为了方便用户删除不需要的照片。
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

#include "delphotodialog.h"
#include <QtGui>

DelPhotoDialog::DelPhotoDialog( QWidget *parent /* = NULL */ )
    : QDialog( parent )
{
    photosLabel    = new QLabel( tr( "人脸图像：" ) );
    photosLineEdit = new QLineEdit;
    photosLineEdit->setReadOnly( true );
    photosLabel->setBuddy( photosLineEdit );
    browsePushButton = new QPushButton( tr( "浏览..." ) );
    delPushButton    = new QPushButton( tr( "删除" ) );
    cancelPushButton = new QPushButton( tr( "取消" ) );

    connect( browsePushButton, SIGNAL( clicked() ), this, SLOT( addPhotos() ) );
    connect( delPushButton, SIGNAL( clicked() ), this, SLOT( delPhotos() ) );
    connect( cancelPushButton, SIGNAL( clicked() ), this, SLOT( close() ) );

    QHBoxLayout *topLayout = new QHBoxLayout;

    topLayout->addWidget( photosLabel );
    topLayout->addWidget( photosLineEdit );
    topLayout->addWidget( browsePushButton );

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    bottomLayout->addStretch();
    bottomLayout->addWidget( delPushButton );
    bottomLayout->addWidget( cancelPushButton );

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addLayout( topLayout );
    mainLayout->addLayout( bottomLayout );

    setWindowTitle( tr( "删除人脸图像" ) );
    setLayout( mainLayout );
}

DelPhotoDialog::~DelPhotoDialog( void )
{

}

// 添加人脸图像
void
DelPhotoDialog::addPhotos( void )
{
    files = QFileDialog::getOpenFileNames(
        this,
        tr( "添加人脸图像" ),
        "./database/faces/images/",
        tr( "人脸图像 ( *.jpg *.bmp *.png )" )
        );
    QStringList::iterator it = files.begin();
    QString faceText;

    while ( it != files.end() )
    {
        faceText += strippedName( *it ) + " ";
        it++;
    }
    photosLineEdit->setText( faceText );
}

// 删除人脸图像
void
DelPhotoDialog::delPhotos( void )
{
    int r = QMessageBox::warning(
        this,
        tr( "警告" ),
        tr( "您是否确认删除选中的所有文件？" ),
        QMessageBox::Yes | QMessageBox::No
        );

    if ( r == QMessageBox::Yes )
    {
        QStringList::iterator it = files.begin();

        while ( it != files.end() )
        {
            QFile file;

            file.remove( *it );
            it++;
        }

        photosLineEdit->clear();
    }
    else
    {
        return;
    }
}

QString
DelPhotoDialog::strippedName( const QString &fullFileName )
{
    return QFileInfo( fullFileName ).fileName();
}
