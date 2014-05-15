/*
 *==================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (c) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：mapwidget.h
 * 文件摘要：本头文件用于声明系统用于查看区域地图的窗口部件
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

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QGraphicsView>

class QLabel;

class MapWidget : public QGraphicsView
{
    Q_OBJECT

public:
     MapWidget( void );
    ~MapWidget( void );

    void    readMap( void );
    QPointF mapToMap( QPointF point );

public slots:
    void slotZoom( int value );

protected:
    void drawBackground( QPainter *painter, const QRectF &rect );
    void mouseMoveEvent( QMouseEvent *event );

private:
    QPixmap  map;
    qreal    zoom;
    QLabel  *viewCoord;
    QLabel  *sceneCoord;
    QLabel  *mapCoord;

    double   x1, y1;
    double   x2, y2;
};

#endif // MAPWIDGET_H
