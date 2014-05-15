/*
 *=================================================================================================
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
 *=================================================================================================
 */

#include "facerecognize.h"
#include <QDebug>
using namespace std;

static int s_maxlikelihood = 0;

FaceRecognize::FaceRecognize( void )
{
    fr_num = 0;

    for ( int i = 0; i < FIND_NUM; i++ )
    {
        fr_threeFirst[i] = -1;
    }

    fr_hmmp.hp_state[0] = 5;
    fr_hmmp.hp_state[1] = 3;
    fr_hmmp.hp_state[2] = 6;
    fr_hmmp.hp_state[3] = 6;
    fr_hmmp.hp_state[4] = 6;
    fr_hmmp.hp_state[5] = 3;
    fr_hmmp.hp_state[6] = 3;
    fr_hmmp.hp_state[7] = 3;

    for ( int i = 0; i < MIXTURE_NUM; i++ )
    {
        fr_hmmp.hp_mixture[i] = 1;
    }

    fr_hmmp.hp_max   = 80;
    fr_hmmp.hp_dct   = cvSize( 12, 12 );
    fr_hmmp.hp_obs   = cvSize( 3, 3 );
    fr_hmmp.hp_delta = cvSize( 4, 4 );
}

FaceRecognize::~FaceRecognize( void )
{
    delAllPersonFromList();
}

int
FaceRecognize::getNum( void )
{
    return fr_num;
}

int *
FaceRecognize::getPos( void )
{
    return fr_threeFirst;
}

char **
FaceRecognize::getResultNames( void )
{
    char                          **name = new char* [fr_num];
    std::list<Person *>::iterator   it;
    int                             current = 0;

    for ( it = fr_personList.begin(); it != fr_personList.end(); it++ )
    {
        if ( current == fr_threeFirst[0] && s_maxlikelihood > MAX_LIKELIHOOD )
        {
            name[0] = (*it)->getName();
            return name;
        }
        current++;
    }

    return NULL;
}

int
FaceRecognize::getResultNum( void )
{
    std::list<Person *>::iterator it;
    int                           current = 0;

    for ( it = fr_personList.begin(); it != fr_personList.end(); it++ )
    {
        if ( current == fr_threeFirst[0] && s_maxlikelihood > MAX_LIKELIHOOD )
        {
            return (*it)->getNum();
        }
        current++;
    }

    return -1;
}

HMM_PARAMS
FaceRecognize::getHMMP( void )
{
    return fr_hmmp;
}

// 获取所有人的信息。
list<Person *>
FaceRecognize::getPersonList( void )
{
    return fr_personList;
}

// 获取某个人的信息。
Person *
FaceRecognize::getPerson( int pos )
{
    list<Person *>::iterator it;
    int                      current = 0;

    for ( it = fr_personList.begin(); it != fr_personList.end(); it++ )
    {
        current++;
        if ( current == pos )
        {
            return *it;
        }
    }

    return NULL;
}

// 添加一个人的信息
void
FaceRecognize::addPersonToList( Person *person )
{
    fr_personList.push_back( person );
}

// 删除某个人的信息。
bool
FaceRecognize::delPersonFromList( int pos )
{
    list<Person *>::iterator it;
    int                      current = 0;

    for ( it = fr_personList.begin(); it != fr_personList.end(); it++ )
    {
        current++;
        if ( current == pos )
        {
            fr_personList.remove( *it );

            return true;
        }
    }

    return false;
}

// 删除所有人的信息。
void
FaceRecognize::delAllPersonFromList( void )
{
    fr_personList.erase( fr_personList.begin(), fr_personList.end() );
}

void
FaceRecognize::setHMMP( HMM_PARAMS &hmmp )
{
    fr_hmmp = hmmp;
}

void
FaceRecognize::trainPerson( int pos )
{
    Face *face = getPerson( pos )->getFace();

    if ( face == NULL )
    {
        qWarning( "FaceRecognize::trainPerson error" );
        return;
    }

    face->trainHMM( fr_hmmp );
}

void
FaceRecognize::trainAll( void )
{
    list<Person *>::iterator it;

    for ( it = fr_personList.begin(); it != fr_personList.end(); it++ )
    {
        (*it)->getFace()->trainHMM( fr_hmmp );
    }
}

void
FaceRecognize::recognize( IplImage *ipl )
{
    float likeArray[1000];
    CvSize cvroi = cvSize(
        ipl->roi ? ipl->roi->width  : ipl->width,
        ipl->roi ? ipl->roi->height : ipl->height
        );
    CvSize        obsNum;
    CvImgObsInfo *info;

    CV_COUNT_OBS( &cvroi, &fr_hmmp.hp_dct, &fr_hmmp.hp_delta, &obsNum );

    int vectLen = fr_hmmp.hp_obs.height * fr_hmmp.hp_obs.width;

    info = cvCreateObsInfo( obsNum, vectLen );
    cvImgToObs_DCT( ipl, info->obs, fr_hmmp.hp_dct, fr_hmmp.hp_obs, fr_hmmp.hp_delta );

    list<Person *>::iterator it;
    Person                  *person;
    int                      perNum = 0;

    for ( it = fr_personList.begin(); it != fr_personList.end(); it++ )
    {
        CvEHMM *hmm = NULL;

        person = *it;

        if ( !person->getFace()->isTrained() )
        {
            fr_num = -1;
            return;
        }
        hmm = person->getFace()->getEHMM();
        if ( hmm == NULL )
        {
            fr_num = -1;
            return;
        }

        cvEstimateObsProb( info, hmm );
        likeArray[perNum++] = cvEViterbi( info, hmm );
    }
    cvReleaseObsInfo( &info );

    for ( int i = 0; i < MIN( FIND_NUM, perNum ); i++ )
    {
        float maxl     = -FLT_MAX;
        int   maxindex = -1;

        for ( int j = 0; j < perNum; j++ )
        {
            if ( likeArray[j] > maxl )
            {
                maxl     = likeArray[j];
                maxindex = j;
            }
        }
        s_maxlikelihood     = maxl;
        qDebug() << "maxl = " << maxl;
        fr_threeFirst[i]    = maxindex;
        likeArray[maxindex] = -FLT_MAX;
    }

    fr_num = MIN( FIND_NUM, perNum );
}

bool
FaceRecognize::savePerson( char *fileName )
{
    FILE *file = NULL;

    file = fopen( fileName, "wt" );
    if ( file == NULL )
    {
        qWarning( "FaceRecognize::savePerson/fopen error" );
        return false;
    }

    list<Person *>::iterator it;

    fprintf( file, "%s %d\n", "<PersonNum>", fr_personList.size() );
    fprintf( file, "%s\n", "[Name]----[NO.]" );
    for ( it = fr_personList.begin(); it != fr_personList.end(); it++ )
    {
        fprintf( file, "%s %d\n", (*it)->getName(), (*it)->getNum() );
    }
    fclose( file );

    return true;
}

bool
FaceRecognize::loadPerson( char *fileName )
{
    FILE *file = NULL;
    char  tempChar[TEMP_SIZE];
    char  nameChar[TEMP_SIZE];
    char  faceChar[TEMP_SIZE];
    int   numInt;

    file = fopen( fileName, "rt" );
    if ( file == NULL )
    {
        qWarning( "FaceRecognize::loadPerson/loadPerson error" );
        return false;
    }

    int personNum = 0;

    fscanf( file, "%s %d\n", tempChar, &personNum );
    fscanf( file, "%s\n", tempChar );
    for ( int i = 0; i < personNum; i++ )
    {
        fscanf( file, "%s %d\n", nameChar, &numInt );

        Face *face = new Face;

        sprintf( faceChar, "./database/faces/hmm_model/%d.hmm", numInt );
        face->load( faceChar );
        Person *person = new Person( numInt, nameChar, face );
        addPersonToList( person );
    }
    fclose( file );

    return true;
}
