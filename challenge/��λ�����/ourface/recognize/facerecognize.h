/*
 *===================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：facerecognize.h
 * 文件摘要：本文件用于声明人脸识别算法的类, 这是系统的核心算法类实现, 其找到最相似的三个人，并输出其姓名。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/02
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *===================================================================================================
 */

#ifndef FACERECOGNIZE_H
#define FACERECOGNIZE_H

#include "face.h"
#include "person.h"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <list>

#define FIND_NUM    1
#define TEMP_SIZE 128

#define MAX_LIKELIHOOD (-52.2000)

class Person;

class FaceRecognize
{
public:
     FaceRecognize( void );
    ~FaceRecognize( void );

    int                 getNum( void );
    int                *getPos( void );
    int                 getResultNum( void );
    char              **getResultNames( void );
    HMM_PARAMS          getHMMP( void );
    std::list<Person *> getPersonList( void );
    Person             *getPerson( int pos );
    void                setHMMP( HMM_PARAMS &hmmp );
    void                addPersonToList( Person *person );
    bool                delPersonFromList( int pos );
    void                delAllPersonFromList( void );

    void                trainPerson( int pos );
    void                trainAll( void );
    void                recognize( IplImage *ipl );

    bool                savePerson( char *fileName );
    bool                loadPerson( char *fileName );

private:
    std::list<Person *> fr_personList;
    HMM_PARAMS          fr_hmmp;
    int                 fr_num;
    int                 fr_threeFirst[FIND_NUM];
};

#endif // FACERECOGNIZE_H
