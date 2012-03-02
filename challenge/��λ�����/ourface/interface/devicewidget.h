/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：devicewidget.h
 * 文件摘要：本头文件用于声明显示设备状态和传感器信息界面的类
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

#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QDateTime>

class QGroupBox;
class QLabel;
class QTimer;
class QLineEdit;
class QLCDNumber;

class DeviceWidget : public QDialog
{
    Q_OBJECT

public:
    DeviceWidget( QWidget *parent = NULL );
    virtual ~DeviceWidget( void );

private slots:
    void refresh( void );

private:
    QGroupBox  *deviceGroupBox;
    QGroupBox  *temperatureGroupBox;  // 温度组合框
    QGroupBox  *humidityGroupBox;     // 湿度组合框
    QGroupBox  *smokeGroupBox;        // 烟雾组合框
    QGroupBox  *noiseGroupBox;        // 噪声组合框
    QGroupBox  *infraredGroupBox;     // 红外组合框
    QGroupBox  *pyroelectricGroupBox; // 热释电组合框
    QGroupBox  *fanGroupBox;          // 电扇控制组合框
    QGroupBox  *lampGroupBox;         // 电灯控制组合框

    QLabel     *temperatureLabel;
    QLineEdit  *temperatureLineEdit;
    QLabel     *humidityLabel;
    QLineEdit  *humidityLineEdit;
    QLabel     *smokeLabel;
    QLineEdit  *smokeLineEdit;
    QLabel     *noiseLabel;
    QLineEdit  *noiseLineEdit;
    QLabel     *infraredLabel;
    QLineEdit  *infraredLineEdit;
    QLabel     *pyroelectricLabel;
    QLineEdit  *pyroelectricLineEdit;
    QLabel     *fanLabel;
    QLineEdit  *fanLineEdit;
    QLabel     *lampLabel;
    QLineEdit  *lampLineEdit;

    QLabel     *dateTimeLabel;
    QLCDNumber *dateTimeLCDNumber;

    QDateTime  dateTime;
    QTimer    *timer;
};

#endif // DEVICEWIDGET_H
