/*
 *=================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：person.h
 * 文件摘要：本文件用于声明人的信息的类。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/01
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *=================================================================================================
 */

#include "person.h"
#include <string.h>
#include <QDebug>
#include <QtGui>

Person::Person( int num, char *name, Face *face )
{
    p_num  = num;
    strncpy( p_name, name, sizeof (name) );
    p_face = face;
}

Person::~Person( void )
{
    delete p_face;
    memset( p_name, 0, sizeof (p_name) );
    p_num = 0;
}

// 为每个人设置一个训练好的人脸
void
Person::setFace( Face *face )
{
    p_face = face;
}

// 为每个人设置一个ID号。
void
Person::setNum( int num )
{
    p_num = num;
}

// 为每个人设置一个名称。
void
Person::setName( char *name )
{
    strncpy( p_name, name, sizeof (name) );
}

// 获取某人的人脸图像
Face *
Person::getFace( void )
{
    return p_face;
}

// 获取某人的编号。
int
Person::getNum( void )
{
    return p_num;
}

// 获取某人的姓名。
char *
Person::getName( void )
{
    return p_name;
}

// 保存某人已经训练完毕的马尔科夫人脸模型。
bool
Person::saveFace( char *fileName )
{
    if ( p_face == NULL )
    {
        qWarning( "Person::saveFace error" );
        return false;
    }

    return p_face->save( fileName );
}

// 加载某人已经训练完毕的马尔科夫人脸模型。
bool
Person::loadFace( char *fileName )
{
    if ( p_face == NULL )
    {
        qWarning( "Person::loadFace error" );
        return false;
    }

    return p_face->load( fileName );
}
