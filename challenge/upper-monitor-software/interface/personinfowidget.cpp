/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：personinfowidget.cpp
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

#include "personinfowidget.h"
#include <QtGui>

PersonInfoWidget::PersonInfoWidget( QWidget *parent /* = NULL */ )
    : QWidget( parent )
{
    faceLabel = new QLabel;

    personGroupBox = new QGroupBox( tr( "人员信息" ) );
    faceLabel->setFixedSize( 200, 140 );
    numLabel  = new QLabel( tr( "编号：" ) );
    nameLabel = new QLabel( tr( "姓名：" ) );
    numLineEdit  = new QLineEdit( tr( "无" ) );
    numLineEdit->setReadOnly( true );
    nameLineEdit = new QLineEdit( tr( "无" ) );
    nameLineEdit->setReadOnly( true );

    QGridLayout *rightLayout = new QGridLayout;

    rightLayout->addWidget( numLabel, 0, 0 );
    rightLayout->addWidget( numLineEdit, 0, 1 );
    rightLayout->addWidget( nameLabel, 1, 0 );
    rightLayout->addWidget( nameLineEdit, 1, 1 );

    QHBoxLayout *groupBoxLayout = new QHBoxLayout;

    groupBoxLayout->addWidget( faceLabel );
    groupBoxLayout->addLayout( rightLayout );
    personGroupBox->setLayout( groupBoxLayout );
    personGroupBox->setStyleSheet(
        "margin: 5px 0px 0px 5px; padding: 10px; border: 1px solid blue; border-radius: 5px"
        );
    faceLabel->setStyleSheet( "border: 2px solid green; border-radius: 5px" );
    numLabel->setStyleSheet( "border: 1px solid green; border-radius: 5px" );
    nameLabel->setStyleSheet( "border: 1px solid green; border-radius: 5px" );
    numLineEdit->setStyleSheet( "border: 1px solid green; border-radius: 5px" );
    nameLineEdit->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget( personGroupBox );

    setLayout( mainLayout );
    setFixedSize( QSize( 600, 190 ) );
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
}

PersonInfoWidget::~PersonInfoWidget( void )
{
}

// 设置人员相片
void
PersonInfoWidget::setFace( const QImage &face )
{
    faceLabel->setPixmap( QPixmap::fromImage( face ).scaled( 200, 140 ) );
}

// 设置人员姓名
void
PersonInfoWidget::setName( const QString &name )
{
    nameLineEdit->setText( name );
}

// 设置人员编号
void
PersonInfoWidget::setNum( int num )
{
    numLineEdit->setText( tr( "%1" ).arg( num ) );
}
