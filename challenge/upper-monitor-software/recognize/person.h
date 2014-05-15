/*
 *===================================================================================================
 *                        2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
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
 *===================================================================================================
 */

#ifndef PERSON_H
#define PERSON_H

#include "face.h"

#define NAME_SIZE 50

class Person
{
public:
     Person( int num, char *name, Face *face );
    ~Person( void );

    void  setFace( Face *face );
    void  setNum( int num );
    void  setName( char *name );
    int   getNum( void );
    char *getName( void );
    Face *getFace( void );

    bool  saveFace( char *fileName );
    bool  loadFace( char *fileName );

private:
    int   p_num;
    char  p_name[NAME_SIZE];
    Face *p_face;
};

#endif // PERSON_H
