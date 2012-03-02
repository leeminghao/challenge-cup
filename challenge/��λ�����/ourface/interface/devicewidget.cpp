/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：devicewidget.cpp
 * 文件摘要：本头文件用于实现显示设备状态和传感器信息界面的类
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/18
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *==================================================================================
 */

#include "devicewidget.h"
#include "capture/client.h"

#include <QDateTime>
#include <QTimer>
#include <QtGui>

extern Client *g_client;
extern bool    g_hasConnectServer;
extern bool    g_hasOpenFan;
extern bool    g_hasOpenLamp;

DeviceWidget::DeviceWidget( QWidget *parent /* = NULL */ )
    : QDialog( parent )
{
    deviceGroupBox     = new QGroupBox( tr( "设备信息" ) );

    temperatureGroupBox = new QGroupBox( tr( "温度信息" ) );
    temperatureLabel    = new QLabel;
    temperatureLabel->setPixmap( QPixmap( ":/config/temperature.png" ).scaled( 32, 32 ) );
    temperatureLineEdit = new QLineEdit( tr( "000.0 'C" ) );
    temperatureLineEdit->setReadOnly( true );

    QHBoxLayout *temperatureLayout = new QHBoxLayout;

    temperatureLayout->addWidget( temperatureLabel );
    temperatureLayout->addWidget( temperatureLineEdit );
    temperatureGroupBox->setLayout( temperatureLayout );
    temperatureGroupBox->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    humidityGroupBox = new QGroupBox( tr( "湿度信息" ) );
    humidityLabel    = new QLabel;
    humidityLabel->setPixmap( QPixmap( ":/config/hum.jpg" ).scaled( 32, 32 ) );
    humidityLineEdit = new QLineEdit( tr( "0.0%" ) );
    humidityLineEdit->setReadOnly( true );

    QHBoxLayout *humidityLayout = new QHBoxLayout;

    humidityLayout->addWidget( humidityLabel );
    humidityLayout->addWidget( humidityLineEdit );
    humidityGroupBox->setLayout( humidityLayout );
    humidityGroupBox->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    smokeGroupBox = new QGroupBox( tr( "烟雾信息" ) );
    smokeLabel    = new QLabel;
    smokeLabel->setPixmap( QPixmap( ":/config/smoke.jpg" ).scaled( 32, 32 ) );
    smokeLineEdit = new QLineEdit( tr( "无烟" ) );
    smokeLineEdit->setReadOnly( true );

    QHBoxLayout *smokeLayout = new QHBoxLayout;

    smokeLayout->addWidget( smokeLabel );
    smokeLayout->addWidget( smokeLineEdit );
    smokeGroupBox->setLayout( smokeLayout );
    smokeGroupBox->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    noiseGroupBox = new QGroupBox( tr( "噪声信息" ) );
    noiseLabel    = new QLabel;
    noiseLabel->setPixmap( QPixmap( ":/device/noise.jpg" ).scaled( 32, 32 ) );
    noiseLineEdit = new QLineEdit( tr( "无噪音" ) );
    noiseLineEdit->setReadOnly( true );

    QHBoxLayout *noiseLayout = new QHBoxLayout;

    noiseLayout->addWidget( noiseLabel );
    noiseLayout->addWidget( noiseLineEdit );
    noiseGroupBox->setLayout( noiseLayout );
    noiseGroupBox->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    infraredGroupBox = new QGroupBox( tr( "红外信息" ) );
    infraredLabel    = new QLabel;
    infraredLabel->setPixmap( QPixmap( ":/device/infrared.jpg" ).scaled( 32, 32 ) );
    infraredLineEdit = new QLineEdit( tr( "无人" ) );
    infraredLineEdit->setReadOnly( true );

    QHBoxLayout *infraredLayout = new QHBoxLayout;

    infraredLayout->addWidget( infraredLabel );
    infraredLayout->addWidget( infraredLineEdit );
    infraredGroupBox->setLayout( infraredLayout );
    infraredGroupBox->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    pyroelectricGroupBox = new QGroupBox( tr( "热释电信息" ) );
    pyroelectricLabel    = new QLabel;
    pyroelectricLabel->setPixmap( QPixmap( ":/device/pyroelectric.jpg" ).scaled( 32, 32 ) );
    pyroelectricLineEdit = new QLineEdit( tr( "无人" ) );
    pyroelectricLineEdit->setReadOnly( true );

    QHBoxLayout *pyroelectricLayout = new QHBoxLayout;

    pyroelectricLayout->addWidget( pyroelectricLabel );
    pyroelectricLayout->addWidget( pyroelectricLineEdit );
    pyroelectricGroupBox->setLayout( pyroelectricLayout );
    pyroelectricGroupBox->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    lampGroupBox = new QGroupBox( tr( "电灯信息" ) );
    lampLabel    = new QLabel;
    lampLabel->setPixmap( QPixmap( ":/device/close_lamp.jpg" ).scaled( 32, 32 ) );
    lampLineEdit = new QLineEdit( tr( "关闭" ) );
    lampLineEdit->setReadOnly( true );

    QHBoxLayout *lampLayout = new QHBoxLayout;

    lampLayout->addWidget( lampLabel );
    lampLayout->addWidget( lampLineEdit );
    lampGroupBox->setLayout( lampLayout );
    lampGroupBox->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    fanGroupBox = new QGroupBox( tr( "电扇信息" ) );
    fanLabel    = new QLabel;
    fanLabel->setPixmap( QPixmap( ":/device/close_fan.jpg" ).scaled( 32, 32 ) );
    fanLineEdit = new QLineEdit( tr( "关闭" ) );
    fanLineEdit->setReadOnly( true );

    QHBoxLayout *fanLayout = new QHBoxLayout;

    fanLayout->addWidget( fanLabel );
    fanLayout->addWidget( fanLineEdit );
    fanGroupBox->setLayout( fanLayout );
    fanGroupBox->setStyleSheet( "border: 1px solid green; border-radius: 5px" );

    QPalette p = palette();

    dateTimeLabel = new QLabel( tr( "日期时间：" ) );
    dateTimeLabel->setStyleSheet( "border: 2px solid green; border-radius: 5px" );
    p.setColor( QPalette::WindowText, Qt::red );
    dateTimeLCDNumber = new QLCDNumber;
    dateTimeLCDNumber->setFixedHeight( 40 );
    dateTimeLCDNumber->setPalette( p );
    dateTimeLCDNumber->setNumDigits( 20 );
    dateTimeLCDNumber->setSegmentStyle( QLCDNumber::Flat );
    dateTimeLCDNumber->setStyleSheet( "border: 2px solid green; border-radius: 5px" );

    QGridLayout *deviceGroupBoxLayout = new QGridLayout;

    deviceGroupBoxLayout->addWidget( dateTimeLabel, 0, 0 );
    deviceGroupBoxLayout->addWidget( dateTimeLCDNumber, 0, 1 );
    deviceGroupBoxLayout->addWidget( temperatureGroupBox, 1, 0 );
    deviceGroupBoxLayout->addWidget( humidityGroupBox, 1, 1 );
    deviceGroupBoxLayout->addWidget( smokeGroupBox, 2, 0 );
    deviceGroupBoxLayout->addWidget( noiseGroupBox, 2, 1 );
    deviceGroupBoxLayout->addWidget( infraredGroupBox, 3, 0 );
    deviceGroupBoxLayout->addWidget( pyroelectricGroupBox, 3, 1 );
    deviceGroupBoxLayout->addWidget( fanGroupBox, 4, 0 );
    deviceGroupBoxLayout->addWidget( lampGroupBox, 4, 1 );

    deviceGroupBox->setLayout( deviceGroupBoxLayout );
    deviceGroupBox->setStyleSheet(
            "margin: 5px 0px 0px 5px; padding: 3px; border: 1px solid blue; border-radius: 5px"
            );

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget( deviceGroupBox );

    timer = new QTimer( this );
    timer->setInterval( 1000 );
    connect( timer, SIGNAL( timeout() ), SLOT( refresh() ) );
    timer->start();

    setLayout( mainLayout );
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
}

DeviceWidget::~DeviceWidget( void )
{
}

// 刷新信息
void
DeviceWidget::refresh( void )
{
    dateTime = QDateTime::currentDateTime();

    dateTimeLCDNumber->display( dateTime.toString( "yyyy:MM:dd hh:mm:ss" ) );
    if ( g_hasConnectServer )
    {
//        temperatureLineEdit->setText( (char *)g_client->getTemperature() );
    }
    if ( g_hasConnectServer )
    {
        if ( g_hasOpenFan )
        {
            fanLabel->setPixmap( QPixmap( ":/device/open_fan.jpg" ).scaled( 32, 32 ) );
            fanLineEdit->setText( tr( "打开" ) );
        }
        else
        {
            fanLabel->setPixmap( QPixmap( ":/device/close_fan.jpg" ).scaled( 32, 32 ) );
            fanLineEdit->setText( tr( "关闭" ) );
        }

        if ( g_hasOpenLamp )
        {
            lampLabel->setPixmap( QPixmap( ":/device/open_lamp.jpg" ).scaled( 32, 32 ) );
            lampLineEdit->setText( tr( "打开" ) );
        }
        else
        {
            lampLabel->setPixmap( QPixmap( ":/device/close_lamp.jpg" ).scaled( 32, 32 ) );
            lampLineEdit->setText( tr( "关闭" ) );
        }
    }
}
