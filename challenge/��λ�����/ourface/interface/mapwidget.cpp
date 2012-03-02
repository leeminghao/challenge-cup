/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：mapwidget.cpp
 * 文件摘要：本头文件用于实现系统用于查看区域地图的窗口部件
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/09
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *==================================================================================
 */

#include "mapwidget.h"
#include <QtGui>

MapWidget::MapWidget( void )
{
    readMap();    // 读取地图信息
    zoom = 50;

    int width  = map.width();
    int height = map.height();

    QGraphicsScene *scene = new QGraphicsScene( this );

    scene->setSceneRect( -width / 2, -height / 2, width, height );
    setScene( scene );
    setCacheMode( CacheBackground );

    // 用于地图缩放的滑动条
    QSlider *slider = new QSlider;

    slider->setOrientation( Qt::Vertical );
    slider->setRange( 1, 100 );
    slider->setTickInterval( 10 );
    slider->setValue( 50 );
    connect( slider, SIGNAL( valueChanged( int ) ), this, SLOT( slotZoom( int ) ) );

    QLabel *zoominLabel = new QLabel;

    zoominLabel->setScaledContents( true );
    zoominLabel->setPixmap( QPixmap( ":/map/zoomin.png" ) );

    QLabel *zoomoutLabel = new QLabel;

    zoomoutLabel->setScaledContents( true );
    zoomoutLabel->setPixmap( QPixmap( ":/map/zoomout.png" ) );

    // 坐标值显示区
    QFrame *coordFrame = new QFrame;
    QLabel *label1     = new QLabel( tr( "GraphicsView: " ) );

    viewCoord = new QLabel;

    QLabel *label2     = new QLabel( tr( "GraphicsScene: " ) );

    sceneCoord = new QLabel;

    QLabel *label3     = new QLabel( tr( "map: " ) );

    mapCoord = new QLabel;

    QGridLayout *northWestLayout = new QGridLayout;

    northWestLayout->addWidget( label1, 0, 0 );
    northWestLayout->addWidget( viewCoord, 0, 1 );
    northWestLayout->addWidget( label2, 1, 0 );
    northWestLayout->addWidget( sceneCoord, 1, 1 );
    northWestLayout->addWidget( label3, 2, 0 );
    northWestLayout->addWidget( mapCoord, 2, 1 );
    northWestLayout->setSizeConstraint( QLayout::SetFixedSize );
    coordFrame->setLayout( northWestLayout );

    // 缩放控制子布局
    QVBoxLayout *zoomLayout = new QVBoxLayout;

    zoomLayout->addWidget( zoominLabel );
    zoomLayout->addWidget( slider );
    zoomLayout->addWidget( zoomoutLabel );

    // 缩放控制子布局
    QVBoxLayout *coordLayout = new QVBoxLayout;

    coordLayout->addWidget( coordFrame );
    coordLayout->addStretch();

    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout;

    mainLayout->addLayout( zoomLayout );
    mainLayout->addLayout( coordLayout );
    mainLayout->addStretch();
    mainLayout->setMargin( 30 );
    mainLayout->setSpacing( 10 );

    setLayout( mainLayout );
    setWindowTitle( tr( "区域地图" ) );
    setMinimumSize( 600, 400 );
}

MapWidget::~MapWidget( void )
{
}

// 读取描述地图信息的文件
void
MapWidget::readMap( void )
{
    QFile   mapFile( "./interface/images/map/map.info" );
    QString mapName;
    int     ok = mapFile.open( QIODevice::ReadOnly );

    if ( ok )
    {
        QTextStream text( &mapFile );

        if ( !text.atEnd() )
        {
            text >> mapName;
            text >> x1 >> y1 >> x2 >> y2;
        }
    }
    map.load( mapName );
}

// 从场景坐标转换至地图坐标
QPointF
MapWidget::mapToMap( QPointF point )
{
    QPointF latLon;

    qreal w   = sceneRect().width();
    qreal h   = sceneRect().height();
    qreal lon = y1 - ((h / 2 + point.y()) * abs( y1 - y2 ) / h );
    qreal lat = x1 + ((w / 2 + point.x()) * abs( x1 - x2 ) / w );

    latLon.setX( lat );
    latLon.setY( lon );

    return latLon;
}

// 实现地图缩放的槽
void
MapWidget::slotZoom( int value )
{
    qreal s;

    if ( value > zoom ) // Zoom In
    {
        s = pow( 1.01, (value - zoom) );
    }
    else
    {
        s = pow( 1 / 1.01, (zoom - value) );
    }

    scale( s, s );
    zoom = value;
}

// 以地图图片重画场景的背景实现地图的显示
void
MapWidget::drawBackground( QPainter *painter, const QRectF &/*rect*/ )
{
    painter->drawPixmap( int( sceneRect().left()), int( sceneRect().top()), map );
}

// 响应鼠标事件
void
MapWidget::mouseMoveEvent( QMouseEvent *event )
{
    // QGraphicsView坐标
    QPoint viewPoint = event->pos();

    viewCoord->setText(
        QString::number( viewPoint.x() ) + "," +
        QString::number( viewPoint.y() )
        );

    // QGrapicsScene坐标
    QPointF scenePoint = mapToScene( viewPoint );

    sceneCoord->setText(
        QString::number( scenePoint.x() ) + "," +
        QString::number( scenePoint.y() )
        );

    QPointF latLon = mapToMap( scenePoint );

    mapCoord->setText(
        QString::number( latLon.x() ) + "," +
        QString::number( latLon.y() )
        );
}
