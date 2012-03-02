/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：main.cpp
 * 文件摘要：本文件实现系统主函数。
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

#include <interface/mainwindow.h>

#include <QtGui>

int
main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    QTextCodec::setCodecForTr( QTextCodec::codecForName( "utf8" ) );
    QPixmap pixmap( ":/splash.jpg" );
    QSplashScreen splash( pixmap );

    splash.show();
    app.processEvents();

    MainWindow   mainWin;

    qApp->setFont( QFont( "wqy-zenhei", 10, QFont::Normal ) );
    mainWin.showMaximized();

    splash.finish( &mainWin );

    return app.exec();
}
