/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：mainwindow.cpp
 * 文件摘要：本文件用于实现系统管理界面的类
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/17
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *==================================================================================
 */

#include "mainwindow.h"
#include "leftwidget.h"
#include "devicewidget.h"
#include "personinfowidget.h"
#include "addpersondialog.h"
#include "findpersondialog.h"
#include "mapwidget.h"
#include "photovideowidget.h"
#include "delphotodialog.h"
#include "capture/client.h"
#include "recognize/facerecognize.h"

#include <QtGui>
#include <QtNetwork>

#include <unistd.h>

// 以下为系统所有使用的全局变量的声明, 主要用于刷新各个设备当前的状态

bool              g_hasConnectServer = false; // 判断是否连接到了视频服务器
bool              g_hasOpenFan       = false; // 判断电扇是否打开
bool              g_hasOpenLamp      = false; // 判断电灯是否打开
bool              g_hasDetectFace    = false; // 是否检测到人脸图像。
bool              g_isStartRecord    = false; // 是否开始录制视频
QString           g_recordVideoName;          // 录制的视频保存的文件名
PersonInfoWidget *g_personInfoWidget;         // 显示识别的人的信息
Client           *g_client           = NULL;  // 请求数据的客户端
FaceRecognize    *g_faceRecognize    = NULL;  // 人脸识别的算法


// 构造函数
MainWindow::MainWindow( void )
{
    sleep( 3 );
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    leftWidget         = new LeftWidget;
    deviceWidget       = new DeviceWidget;
    delPhotoDialog     = new DelPhotoDialog;
    // 全局变量定义
    g_personInfoWidget = new PersonInfoWidget;
    g_client           = new Client;        // 创建全局变量
    g_faceRecognize    = new FaceRecognize;  // 创建全局变量
    g_faceRecognize->loadPerson( (char *)"./database/info/person.info" );

    rightSplitter = new QSplitter( Qt::Vertical );
    rightSplitter->addWidget( g_personInfoWidget );
    rightSplitter->addWidget( deviceWidget );
    rightSplitter->setStretchFactor( 1, 2 );

    mainSplitter = new QSplitter( Qt::Horizontal );
    mainSplitter->addWidget( leftWidget );
    mainSplitter->addWidget( rightSplitter );
    mainSplitter->setStretchFactor( 2, 1 );
    setCentralWidget( mainSplitter );

    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    setWindowIcon( QIcon( ":/face.png" ) );
    setWindowTitle( tr( "便携式HMM远程面部检测识别综合控制应用平台" ) );
}

// 析构函数
MainWindow::~MainWindow( void )
{
    delete leftWidget;
    delete deviceWidget;
    delete g_personInfoWidget;
    delete g_client;
    delete g_faceRecognize;
}

void
MainWindow::closeEvent( QCloseEvent *event )
{
    int r = QMessageBox::warning(
        this,
        tr( "便携式HMM远程人脸检测识别综合控制应用平台" ),
        tr( "你是否确认关闭应用程序?" ),
        QMessageBox::Yes | QMessageBox::No
        );

    if ( r == QMessageBox::No )
    {
        event->ignore();
    }
    else if ( r == QMessageBox::Yes )
    {
        g_client->sendControl( END );
        event->accept();
    }
}

// 增加人员
void
MainWindow::addPerson( void )
{
    addPersonDialog = new AddPersonDialog;

    addPersonDialog->show();
}

// 修改人员
void
MainWindow::modifyPerson( void )
{

}

// 查找人员
void
MainWindow::findPerson( void )
{
    findPersonDialog = new FindPersonDialog;

    findPersonDialog->show();
}

// 删除人员
void
MainWindow::delPerson( void )
{

}

// 查看区域地图信息
void
MainWindow::showAreaMap( void )
{
    mapWidget = new MapWidget;

    mapWidget->show();
}

// 获取本机网络信息
void
MainWindow::seeLocalNetwork( void )
{
    QString                  detail = "";
    QList<QNetworkInterface> list   = QNetworkInterface::allInterfaces();

    for ( int i = 0; i < list.count(); i++ )
    {
        QNetworkInterface interface = list.at( i );

        detail += tr( "设备：" ) + interface.name() + "\n";

        QString hardwareAddress = interface.hardwareAddress();

        detail += tr( "硬件地址：" ) + interface.hardwareAddress() + "\n";

        QList<QNetworkAddressEntry> entryList = interface.addressEntries();

        for ( int j = 0; j < entryList.count(); j++ )
        {
            QNetworkAddressEntry entry = entryList.at( j );

            detail += "\t" + tr( "IP地址：" ) + entry.ip().toString() + "\n";
            detail += "\t" + tr( "子网掩码：" ) + entry.netmask().toString() + "\n";
            detail += "\t" + tr( "广播地址：" ) + entry.broadcast().toString() + "\n";
        }
    }
    QMessageBox::information( this, tr( "详细信息" ), detail );
}

void
MainWindow::connectServer( void )
{
    if ( g_client->connectServer() )
    {
        g_hasConnectServer = true;
        temperatureAction->setEnabled( true );
        humAction->setEnabled( true );
        smokeAction->setEnabled( true );
        openFanAction->setEnabled( true );
        openLampAction->setEnabled( true );
        takePhotoAction->setEnabled( true );
        delPhotoAction->setEnabled( true );
        startRecordAction->setEnabled( true );
        stopRecordAction->setEnabled( true );
        statusBar()->showMessage( tr( "连接到服务器成功!" ) );
    }
    else
    {
        statusBar()->showMessage( tr( "连接到服务器失败！" ) );
    }
}

// 配置温感参数
void
MainWindow::temperature( void )
{

}

// 配置湿感参数
void
MainWindow::hum( void )
{

}

// 配置烟感参数
void
MainWindow::smoke( void )
{

}

// 拍照
void
MainWindow::takePhoto( void )
{
    leftWidget->getPhotoVideoWidget()->takePhoto();
    statusBar()->showMessage( tr( "拍照成功!" ) );
}

// 删除照片
void
MainWindow::delPhoto( void )
{
    delPhotoDialog = new DelPhotoDialog;

    delPhotoDialog->show();
}

// 开始录制视频
void
MainWindow::startRecord( void )
{
}

// 停止录制视频
void
MainWindow::stopRecord( void )
{
}

// 打开电扇
void
MainWindow::openFan( void )
{
    if ( g_hasConnectServer )
    {
        if ( g_client->sendControl( OPEN_FAN ) )
        {
            g_hasOpenFan = true;
            openFanAction->setEnabled( false );
            closeFanAction->setEnabled( true );
            statusBar()->showMessage( tr( "打开电扇成功!" ) );
        }
        else
        {
            statusBar()->showMessage( tr( "打开电扇失败!" ) );
        }
    }
}

// 关闭电扇
void
MainWindow::closeFan( void )
{
    if ( g_hasConnectServer )
    {
        if ( g_client->sendControl( CLOSE_FAN ) )
        {
            g_hasOpenFan = false;
            openFanAction->setEnabled( true );
            closeFanAction->setEnabled( false );
            statusBar()->showMessage( tr( "关闭电扇成功!" ) );
        }
        else
        {
            statusBar()->showMessage( tr( "关闭电扇失败!" ) );
        }
    }
}

// 打开电灯
void
MainWindow::openLamp( void )
{
    if ( g_hasConnectServer )
    {
        if ( g_client->sendControl( OPEN_LAMP ) )
        {
            g_hasOpenLamp = true;
            openLampAction->setEnabled( false );
            closeLampAction->setEnabled( true );
            statusBar()->showMessage( tr( "打开电灯成功!" ) );
        }
        else
        {
            statusBar()->showMessage( tr( "打开电灯失败!" ) );
        }
    }
}

// 关闭电灯
void
MainWindow::closeLamp( void )
{
    if ( g_hasConnectServer )
    {
        if ( g_client->sendControl( CLOSE_LAMP ) )
        {
            g_hasOpenLamp = false;
            openLampAction->setEnabled( true );
            closeLampAction->setEnabled( false );
            statusBar()->showMessage( tr( "关闭电扇成功!" ) );
        }
        else
        {
            statusBar()->showMessage( tr( "关闭电扇失败!" ) );
        }
    }
}

void
MainWindow::about( void )
{
    QMessageBox::about(
            this,
            tr( "关于便携式HMM远程面部检测识别综合控制应用平台" ),
            tr( "<p><b>便携式HMM远程面部检测识别综合控制应用平台</b></p><p></p>"
                "<p>版本 0.1 (2011-04-10)</p><p></p>"
                "<p>开发者: 北华航天工业学院 汪金伟/王旭影/李明浩 (1056531028@qq.com)</p><p></p>"
                "<p>课题负责人：李明浩(15133676537)</p><p></p>"
                "<p>获取更多信息, 请联系作者</p>"
              )
            );
}

void
MainWindow::help( void )
{

}

// 创建菜单条
void
MainWindow::createActions( void )
{
    addPersonAction = new QAction( tr( "新增(&N)" ), this );
    addPersonAction->setIcon( QIcon( ":/person/add.png" ) );
    addPersonAction->setShortcut( QKeySequence::New );
    addPersonAction->setStatusTip( tr( "增加人员信息" ) );
    connect( addPersonAction, SIGNAL( triggered() ), this, SLOT( addPerson() ) );

    modifyPersonAction = new QAction( tr( "修改(&M)" ), this );
    modifyPersonAction->setIcon( QIcon( ":/person/modify.png" ) );
    modifyPersonAction->setShortcut( tr( "Ctrl+M" ) );
    modifyPersonAction->setStatusTip( tr( "修改人员信息" ) );
    connect( modifyPersonAction, SIGNAL( triggered() ), this, SLOT( modifyPerson() ) );

    findPersonAction = new QAction( tr( "识别(&R)" ), this );
    findPersonAction->setIcon( QIcon( ":/person/find.png" ) );
    findPersonAction->setShortcut( tr( "Ctrl+R" ) );
    findPersonAction->setStatusTip( tr( "使用人脸图像查找人员信息" ) );
    connect( findPersonAction, SIGNAL( triggered() ), this, SLOT( findPerson() ) );

    delPersonAction = new QAction( tr( "删除(&D)" ), this );
    delPersonAction->setIcon( QIcon( ":/person/del.png" ) );
    delPersonAction->setShortcut( tr( "Ctrl+D" ) );
    delPersonAction->setStatusTip( tr( "删除人员信息" ) );
    connect( delPersonAction, SIGNAL( triggered() ), this, SLOT( delPerson() ) );

    exitAction = new QAction( tr( "退出(&Z)" ), this );
    exitAction->setIcon( QIcon( ":/person/exit.png" ) );
    exitAction->setShortcut( tr( "Ctrl+Z" ) );
    exitAction->setStatusTip( tr( "退出应用程序" ) );
    connect( exitAction, SIGNAL( triggered() ), this, SLOT( close() ) );

    areaMapAction = new QAction( tr( "显示区域地图" ), this );
    areaMapAction->setIcon( QIcon( ":/map/map.png" ) );
    areaMapAction->setShortcut( tr( "Ctrl+M" ) );
    areaMapAction->setStatusTip( tr( "查看管理区域的地图" ) );
    connect( areaMapAction, SIGNAL( triggered() ), this, SLOT( showAreaMap() ) );

    seeNetworkAction = new QAction( tr( "获取本机网络信息" ), this );
    seeNetworkAction->setShortcut( tr( "Ctrl+N" ) );
    seeNetworkAction->setStatusTip( tr( "获取本机网络信息" ) );
    connect( seeNetworkAction, SIGNAL( triggered() ), this, SLOT( seeLocalNetwork() ) );

    connectServerAction = new QAction( tr( "连接服务器" ), this );
    connectServerAction->setShortcut( tr( "Ctrl+V" ) );
    connectServerAction->setStatusTip( tr( "连接到服务器" ) );
    connect( connectServerAction, SIGNAL( triggered() ), this, SLOT( connectServer() ) );

    temperatureAction = new QAction( tr( "配置温度传感器(&T)" ), this );
    temperatureAction->setIcon( QIcon( ":/config/temperature.png" ) );
    temperatureAction->setShortcut( tr( "Ctrl+T" ) );
    temperatureAction->setStatusTip( tr( "配置温度传感器系数" ) );
    temperatureAction->setEnabled( false );
    connect( temperatureAction, SIGNAL( triggered() ), this, SLOT( temperature() ) );

    humAction = new QAction( tr( "配置湿度传感器(&H)" ), this );
    humAction->setIcon( QIcon( ":/config/hum.jpg" ) );
    humAction->setShortcut( tr( "Ctrl+H" ) );
    humAction->setStatusTip( tr( "配置湿度传感器系数" ) );
    humAction->setEnabled( false );
    connect( humAction, SIGNAL( triggered() ), this, SLOT( hum() ) );

    smokeAction = new QAction( tr( "配置烟雾传感器(&S)" ), this );
    smokeAction->setIcon( QIcon( ":/config/smoke.jpg" ) );
    smokeAction->setShortcut( tr( "Ctrl+S" ) );
    smokeAction->setStatusTip( tr( "配置烟雾传感器系数" ) );
    smokeAction->setEnabled( false );
    connect( smokeAction, SIGNAL( triggered() ), this, SLOT( smoke() ) );

    takePhotoAction = new QAction( tr( "拍照(&P)" ), this );
    takePhotoAction->setIcon( QIcon( ":/capture/take.png" ) );
    takePhotoAction->setShortcut( tr( "Ctrl+P" ) );
    takePhotoAction->setStatusTip( tr( "拍摄人脸照片" ) );
    takePhotoAction->setEnabled( false );
    connect( takePhotoAction, SIGNAL( triggered() ), this, SLOT( takePhoto() ) );

    delPhotoAction = new QAction( tr( "删除照片(&X)" ), this );
    delPhotoAction->setIcon( QIcon( ":/capture/del.png" ) );
    delPhotoAction->setShortcut( tr( "Ctrl+X" ) );
    delPhotoAction->setStatusTip( tr( "删除人脸照片" ) );
    delPhotoAction->setEnabled( false );
    connect( delPhotoAction, SIGNAL( triggered() ), this, SLOT( delPhoto() ) );

    startRecordAction = new QAction( tr( "开始录制(&R)" ), this );
    startRecordAction->setIcon( QIcon( ":/video/start.png" ) );
    startRecordAction->setShortcut( tr( "Ctrl+R" ) );
    startRecordAction->setStatusTip( tr( "开始录制视频" ) );
    startRecordAction->setEnabled( false );
    connect( startRecordAction, SIGNAL( triggered() ), this, SLOT( startRecord() ) );

    stopRecordAction = new QAction( tr( "停止录制(&E)" ), this );
    stopRecordAction->setIcon( QIcon( ":/video/stop.png" ) );
    stopRecordAction->setShortcut( tr( "Ctrl+E" ) );
    stopRecordAction->setStatusTip( tr( "停止录制视频" ) );
    stopRecordAction->setEnabled( false );
    connect( stopRecordAction, SIGNAL( triggered() ), this, SLOT( stopRecord() ) );

    openFanAction = new QAction( tr( "打开电扇(&O)" ), this );
    openFanAction->setIcon( QIcon( ":/device/open_fan.jpg" ) );
    openFanAction->setShortcut( tr( "Ctrl+O" ) );
    openFanAction->setStatusTip( tr( "打开电扇" ) );
    openFanAction->setEnabled( false );
    connect( openFanAction, SIGNAL( triggered() ), this, SLOT( openFan() ) );

    closeFanAction = new QAction( tr( "关闭电扇(&W)" ), this );
    closeFanAction->setIcon( QIcon( ":/device/close_fan.jpg" ) );
    closeFanAction->setShortcut( tr( "Ctrl+W" ) );
    closeFanAction->setStatusTip( tr( "关闭电扇" ) );
    closeFanAction->setEnabled( false );
    connect( closeFanAction, SIGNAL( triggered() ), this, SLOT( closeFan() ) );

    openLampAction = new QAction( tr( "打开电灯(&L)" ), this );
    openLampAction->setIcon( QIcon( ":/device/open_lamp.jpg" ) );
    openLampAction->setShortcut( tr( "Ctrl+L" ) );
    openLampAction->setStatusTip( tr( "打开电灯" ) );
    openLampAction->setEnabled( false );
    connect( openLampAction, SIGNAL( triggered() ), this, SLOT( openLamp() ) );

    closeLampAction = new QAction( tr( "关闭电灯(&R)" ), this );
    closeLampAction->setIcon( QIcon( ":/device/close_lamp.jpg" ) );
    closeLampAction->setShortcut( tr( "Ctrl+R" ) );
    closeLampAction->setStatusTip( tr( "关闭电灯" ) );
    closeLampAction->setEnabled( false );
    connect( closeLampAction, SIGNAL( triggered() ), this, SLOT( closeLamp() ) );

    aboutAction = new QAction( tr( "关于(&A)" ), this );
    aboutAction->setStatusTip( tr( "关于软件版权" ) );
    connect( aboutAction, SIGNAL( triggered() ), this, SLOT( about() ) );

    aboutQtAction = new QAction( tr( "关于Qt(&Q)" ), this );
    aboutQtAction->setStatusTip( tr( "显示Qt库版权信息" ) );
    connect( aboutQtAction, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

    helpAction = new QAction( tr( "帮助信息(&I)" ), this );
    helpAction->setStatusTip( tr( "系统使用帮助说明" ) );
    connect( helpAction, SIGNAL( triggered() ), this, SLOT( help() ) );
}

void
MainWindow::createMenus( void )
{
    personMenu = menuBar()->addMenu( tr( "人员管理(&P)" ) );
    personMenu->addAction( addPersonAction );
    personMenu->addAction( modifyPersonAction );
    personMenu->addAction( findPersonAction );
    personMenu->addAction( delPersonAction );
    personMenu->addSeparator();
    personMenu->addAction( exitAction );

    areaMapMenu = menuBar()->addMenu( tr( "地区(&A)" ) );
    areaMapMenu->addAction( areaMapAction );

    connectMenu = menuBar()->addMenu( tr( "连接(&L)" ) );
    connectMenu->addAction( seeNetworkAction );
    connectMenu->addSeparator();
    connectMenu->addAction( connectServerAction );

    configMenu = menuBar()->addMenu( tr( "配置(&C)" ) );
    configMenu->addAction( temperatureAction );
    configMenu->addAction( humAction );
    configMenu->addAction( smokeAction );

    captureMenu = menuBar()->addMenu( tr( "捕捉(&B)" ) );
    captureMenu->addAction( takePhotoAction );
    captureMenu->addAction( delPhotoAction );

    videoMenu = menuBar()->addMenu( tr( "视频(&V)" ) );
    videoMenu->addAction( startRecordAction );
    videoMenu->addAction( stopRecordAction );

    deviceMenu = menuBar()->addMenu( tr( "设备(&D)" ) );
    fanSubMenu = deviceMenu->addMenu( tr( "电扇(&F)" ) );
    fanSubMenu->addAction( openFanAction );
    fanSubMenu->addAction( closeFanAction );
    lampSubMenu = deviceMenu->addMenu( tr( "电灯(&L)" ) );
    lampSubMenu->addAction( openLampAction );
    lampSubMenu->addAction( closeLampAction );

    helpMenu = menuBar()->addMenu( tr( "帮助(&H)" ) );
    helpMenu->addAction( helpAction );
    helpMenu->addSeparator();
    helpMenu->addAction( aboutAction );
    helpMenu->addAction( aboutQtAction );
}

// 创建工具条
void
MainWindow::createToolBars( void )
{
    personToolBar = addToolBar( tr( "人员管理(&P)" ) );
    personToolBar->addAction( exitAction );
    personToolBar->addSeparator();
    personToolBar->addAction( addPersonAction );
    personToolBar->addAction( modifyPersonAction );
    personToolBar->addAction( findPersonAction );
    personToolBar->addAction( delPersonAction );

    areaMapToolBar = addToolBar( tr( "区域地图" ) );
    areaMapToolBar->addAction( areaMapAction );

    configToolBar = addToolBar( tr( "配置(&C)" ) );
    configToolBar->addAction( temperatureAction );
    configToolBar->addAction( humAction );
    configToolBar->addAction( smokeAction );

    captureToolBar = addToolBar( tr( "捕捉(&B)" ) );
    captureToolBar->addAction( takePhotoAction );
    captureToolBar->addAction( delPhotoAction );

    videoToolBar = addToolBar( tr( "视频(&V)" ) );
    videoToolBar->addAction( startRecordAction );
    videoToolBar->addAction( stopRecordAction );

    deviceToolBar = addToolBar( tr( "设备(&D)" ) );
    deviceToolBar->addAction( openFanAction );
    deviceToolBar->addAction( closeFanAction );
    deviceToolBar->addAction( openLampAction );
    deviceToolBar->addAction( closeLampAction );
}

// 创建状态栏
void
MainWindow::createStatusBar( void )
{
    infoLabel = new QLabel( tr( "欢迎使用便携式HMM远程面部检测识别综合控制应用平台——北华航天工业学院计算机科学与技术" ) );
    infoLabel->setAlignment( Qt::AlignLeft );
    infoLabel->setMinimumSize( infoLabel->sizeHint() );

    statusBar()->addWidget( infoLabel );
}
