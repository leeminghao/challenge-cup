/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：mainwindow.h
 * 文件摘要：本头文件用于声明系统管理界面的类
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QAction;
class QTextEdit;
class QSplitter;
class DeviceWidget;
class LeftWidget;
class PersonInfoWidget;
class MapWidget;
class Connect;
class AddPersonDialog;
class FindPersonDialog;
class DelPhotoDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( void );
    virtual ~MainWindow( void );

protected:
    void closeEvent( QCloseEvent *event );

private slots:
    void addPerson( void );        // 增加人员
    void modifyPerson( void );     // 修改人员
    void findPerson( void );       // 查找人员
    void delPerson( void );        // 删除人员

    void showAreaMap( void );      // 查看管理区域地图

    void seeLocalNetwork( void );  // 获取本机网络信息
    void connectServer( void );    // 连接到服务器

    void temperature( void );      // 配置温感参数
    void hum( void );              // 配置湿感参数
    void smoke( void );            // 配置烟感参数

    void takePhoto( void );        // 拍照
    void delPhoto( void );         // 删除照片

    void startRecord( void );      // 开始录制视频
    void stopRecord( void );       // 停止录制视频

    void openFan( void );          // 打开电扇
    void closeFan( void );         // 关闭电扇
    void openLamp( void );         // 打开电灯
    void closeLamp( void );        // 关闭电灯

    void about( void );
    void help( void );

private:
    void createActions( void );
    void createMenus( void );
    void createToolBars( void );
    void createStatusBar( void );

    LeftWidget       *leftWidget;
    DeviceWidget     *deviceWidget;

    AddPersonDialog  *addPersonDialog;
    FindPersonDialog *findPersonDialog;
    MapWidget        *mapWidget;
    DelPhotoDialog   *delPhotoDialog;

    QLabel           *infoLabel;
    QSplitter        *mainSplitter;
    QSplitter        *rightSplitter;

    QMenu *personMenu;
    QMenu *areaMapMenu;
    QMenu *connectMenu;
    QMenu *configMenu;
    QMenu *captureMenu;
    QMenu *videoMenu;
    QMenu *deviceMenu;
    QMenu *helpMenu;
    QMenu *fanSubMenu;
    QMenu *lampSubMenu;

    QToolBar *personToolBar;
    QToolBar *areaMapToolBar;
    QToolBar *configToolBar;
    QToolBar *captureToolBar;
    QToolBar *videoToolBar;
    QToolBar *deviceToolBar;
    QToolBar *helpToolBar;

    QAction *addPersonAction;
    QAction *modifyPersonAction;
    QAction *findPersonAction;
    QAction *delPersonAction;
    QAction *exitAction;

    QAction *areaMapAction;

    QAction *seeNetworkAction;
    QAction *connectServerAction;

    QAction *temperatureAction;
    QAction *humAction;
    QAction *smokeAction;

    QAction *takePhotoAction;
    QAction *delPhotoAction;

    QAction *startRecordAction;
    QAction *stopRecordAction;

    QAction *openFanAction;
    QAction *closeFanAction;
    QAction *openLampAction;
    QAction *closeLampAction;

    QAction *aboutAction;
    QAction *aboutQtAction;

    QAction *helpAction;
};

#endif // MAINWINDOW_H
