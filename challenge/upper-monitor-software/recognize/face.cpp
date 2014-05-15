/*
 *==================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：face.cpp
 * 文件摘要：本文件用于实现人脸的类, 利用人脸图片信息进行训练。
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/04/01
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *==================================================================================================
 */

#include "face.h"
#include <errno.h>
#include <QDebug>
using namespace std;

Face::Face( void )
{
    f_hmm      = NULL;
    f_trained  = false;
    f_vectSize = 0;
}

Face::~Face( void )
{
    delAllImageFromList();
    release();
    f_vectSize = 0;
}

// 释放马尔科夫模型所占用空间。
bool
Face::release( void )
{
    if ( f_hmm != NULL )
    {
        cvRelease2DHMM( &f_hmm );
        f_hmm = NULL;
    }

    return true;
}

// 获取马尔科夫模型空间。
CvEHMM *
Face::getEHMM( void )
{
    return f_hmm;
}

// 获取图像链
list<char *>
Face::getImageList( void )
{
    return f_imageList;
}

// 加入人脸图像到图像链表中。
void
Face::addImageToList( char *imagename )
{
    f_imageList.push_back( imagename );
    f_trained = false;
}

// 从图像链表中删除人脸图像。
bool
Face::delImageFromList( int pos )
{
    list<char *>::iterator it;
    int                    current = 0;

    for ( it = f_imageList.begin(); it != f_imageList.end(); it++ )
    {
        current++;
        if ( current == pos )
        {
            f_imageList.remove( *it );
            f_trained = false;

            return true;
        }
    }

    return false;
}

// 删除所有人脸链表中的人脸图像
void
Face::delAllImageFromList( void )
{
    f_imageList.erase( f_imageList.begin(), f_imageList.end() );
    f_trained = false;
}

// 判断人脸是否已经训练完毕。
bool
Face::isTrained( void )
{
    return f_trained;
}

// 训练人脸图像。
void
Face::trainHMM( HMM_PARAMS &params )
{
    if ( !release() )
    {
        return;
    }

    int vectLen = params.hp_obs.height * params.hp_obs.width;

    f_vectSize = vectLen;
    f_hmm      = cvCreate2DHMM( params.hp_state, params.hp_mixture, vectLen );
    if ( f_hmm == NULL )
    {
        qWarning( "Face::trainHMM/cvCreate2DHMM error" );
        exit( -1 );
    }

    int imgNum = f_imageList.size();

    CvImgObsInfo **obsInfoArray = new CvImgObsInfo* [imgNum];

    list<char *>::iterator it;
    int                    current = 0;

    for ( it = f_imageList.begin(); it != f_imageList.end(); it++ )
    {
        IplImage *ipl = cvLoadImage( *it, CV_LOAD_IMAGE_GRAYSCALE );

        if ( ipl == NULL )
        {
            qWarning( "Face::trainHMM/cvLoadImage error" );
            return;
        }

        CvSize obsNum;
        CvSize roi = cvSize(
            ipl->roi ? ipl->roi->width  : ipl->width,
            ipl->roi ? ipl->roi->height : ipl->height
            );

        CV_COUNT_OBS( &roi, &(params.hp_dct), &(params.hp_delta), &obsNum );
        obsInfoArray[current] = cvCreateObsInfo( obsNum, vectLen );

        CvImgObsInfo *info = obsInfoArray[current];

        // 从图像中提取的观测向量，即DCT系数
        cvImgToObs_DCT( ipl, info->obs, params.hp_dct, params.hp_obs, params.hp_delta );
        // 以HMM状态统一分割图像观测值
        cvUniformImgSegm( info, f_hmm );
        current++;
    }

    // 由混合分量来分割HMM的每个内在状态的所有观测值
    cvInitMixSegm( obsInfoArray, imgNum, f_hmm );

    bool  trained       = false;
    float oldLikelihood = 0.0;
    int   counter       = 0;

    while ( (!trained) && (counter < params.hp_max) )
    {
        counter++;
        // 计算每个HMM状态中的所有内在参数
        cvEstimateHMMStateParams( obsInfoArray, imgNum, f_hmm );
        // 应用现有的图像观测值分割为所有嵌入式和内部的HMM函数，计算可能的变换矩阵
        cvEstimateTransProb( obsInfoArray, imgNum, f_hmm );

        float likelihood = 0.0;

        for ( int i = 0; i < imgNum; i++ )
        {
            cvEstimateObsProb( obsInfoArray[i], f_hmm );
            likelihood += cvEViterbi( obsInfoArray[i], f_hmm );
        }
        likelihood /= imgNum * obsInfoArray[0]->obs_size;
        cvMixSegmL2( obsInfoArray, imgNum, f_hmm );
        trained = (fabs( likelihood - oldLikelihood ) < 0.01);
        oldLikelihood = likelihood;
    }

    for ( int i = 0; i < imgNum; i++ )
    {
        cvReleaseObsInfo( &(obsInfoArray[i]) );
    }

    f_trained = true;
}

bool
Face::load( char *fileName )
{
    FILE *file = NULL;
    int   states[TEMP_SIZE];
    int   mixture[TEMP_SIZE];
    char  temp[TEMP_SIZE];
    char *faceName;

    if ( f_hmm != NULL )
    {
        cvRelease2DHMM( &f_hmm );
    }
    file = fopen( fileName, "rt" );
    if ( file == NULL )
    {
        qWarning( "Face::load/fopen error: %s", strerror( errno ) );
        return false;
    }

    int imgNum = 0;

    // 读取图像信息
    fscanf( file, "%s %d\n", temp, &imgNum );
    fscanf( file, "%s\n", temp );
    for ( int i = 0; i < imgNum; i++ )
    {
        faceName = new char[MAX_PATH];
        fscanf( file, "%s\n", faceName );
        addImageToList( faceName );
    }

    // Read topology
    fscanf( file, "%s %d\n", temp, states );
    fscanf( file, "%s ", temp );
    for ( int i = 0; i < states[0]; i++ )
    {
        fscanf( file, "%d ", states + i + 1 );
    }
    fscanf( file, "\n" );

    int totalStates = 0;

    for ( int i = 0; i < states[0]; i++ )
    {
        totalStates += states[i+1];
    }

    // 读取混合高斯因子的数目
    fscanf( file, "%s ", temp );
    for ( int i = 0; i < totalStates; i++ )
    {
        fscanf( file, "%d ", &mixture[i] );
    }
    fscanf( file, "\n" );

    fscanf( file, "%s %d\n", temp, &f_vectSize );

    f_hmm = cvCreate2DHMM( states, mixture, f_vectSize );
    if ( f_hmm == NULL )
    {
        qWarning( "Face::load/cvCreate2DHMM error" );
        return false;
    }

    CvEHMM *hmm = f_hmm;

    for ( int i = 0; i < states[0] + 1; i++ )
    {
        fscanf( file, "%s\n", temp );

        int tempInt;

        fscanf( file, "%s %d\n", temp, &tempInt );
        assert( tempInt == states[i] );

        if ( i )
        {
            for ( int j = 0; j < states[i]; j++ )
            {
                CvEHMMState *state = &(hmm->u.state[j]);

                fscanf( file, "%s %d\n", temp, &tempInt );
                assert( tempInt == j );

                fscanf( file, "%s %d\n", temp, &tempInt );
                assert( tempInt == state->num_mix );

                float *mu     = state->mu;
                float *invVar = state->inv_var;

                for ( int m = 0; m < state->num_mix; m++ )
                {
                    int tempInt;

                    fscanf( file, "%s %d %s %f\n", temp, &tempInt, temp, &(state->weight[m]) );
                    assert( tempInt == m );
                    fscanf( file, "%s\n", temp );

                    for ( int k = 0; k < f_vectSize; k++ )
                    {
                        fscanf( file, "%f ", mu );
                        mu++;
                    }
                    fscanf( file, "\n" );

                    fscanf( file, "%s\n", temp );

                    for ( int k = 0; k < f_vectSize; k++ )
                    {
                        fscanf( file, "%f ", invVar );
                        invVar++;
                    }
                    fscanf( file, "\n" );

                    fscanf( file, "%s %f\n", temp, &(state->log_var_val[m]) );
                }
            }
        }

        // 读取转置矩阵
        fscanf( file, "%s\n", temp );

        float *prob = hmm->transP;

        for ( int j = 0; j < hmm->num_states; j++ )
        {
            for ( int k = 0; k < hmm->num_states; k++ )
            {
                fscanf( file, "%f ", prob );
                prob++;
            }
            fscanf( file, "\n" );
        }

        fscanf( file, "%s\n", temp );

        hmm = &(f_hmm->u.ehmm[i]);
    }

    fclose( file );
    f_trained = true;

    return true;
}

// 存储训练完毕后的人脸隐马尔科夫模型
bool
Face::save( char *fileName )
{
    FILE *file = NULL;

    if ( f_hmm == NULL )
    {
        return false;
    }

    file = fopen( fileName, "wt" );
    if ( file == NULL )
    {
        qWarning( "Face::save/fopen error: %s", strerror( errno ) );
        return false;
    }

    // write imageList
    list<char *>::iterator it;

    fprintf( file, "%s %d\n", "<Image_Num>", f_imageList.size() );
    fprintf( file, "%s\n", "<Image_Path>" );
    for ( it = f_imageList.begin(); it != f_imageList.end(); it++ )
    {
        fprintf( file, "%s\n", *it );
    }

    // write topology
    fprintf( file, "%s %d\n", "<NumSuperStates>", f_hmm->num_states );
    fprintf( file, "%s ", "<NumStates>" );
    for ( int i = 0; i < f_hmm->num_states; i++ )
    {
        fprintf( file, "%d ", f_hmm->u.ehmm[i].num_states );
    }
    fprintf( file, "\n" );

    fprintf( file, "%s ", "<NumMixtures>" );
    for ( int i = 0; i < f_hmm->num_states; i++ )
    {
        CvEHMM *ehmm = &(f_hmm->u.ehmm[i]);

        for ( int j = 0; j < ehmm->num_states; j++ )
        {
            fprintf( file, "%d ", ehmm->u.state[j].num_mix );
        }
    }
    fprintf( file, "\n" );

    fprintf( file, "%s %d\n", "<VecSize>", f_vectSize );

    // consequently write all hmms
    CvEHMM *hmm = f_hmm;

    for ( int i = 0; i < f_hmm->num_states + 1; i++ )
    {
        if ( hmm->level == 0 )
        {
            fprintf( file, "%s\n", "<BeginEmbeddedHMM>" );
        }
        else
        {
            fprintf( file, "%s\n", "<BeginExternalHMM>" );
        }

        fprintf( file, "%s %d\n", "<NumStates>", hmm->num_states );

        if ( hmm->level == 0 )
        {
            for ( int j = 0; j < hmm->num_states; j++ )
            {
                CvEHMMState *state = &(hmm->u.state[j]);

                fprintf( file, "%s %d\n", "<State>", j );
                fprintf( file, "%s %d\n", "<NumMixes>", state->num_mix );

                float *mu     = state->mu;
                float *invVar = state->inv_var;

                for ( int m = 0; m < state->num_mix; m++ )
                {
                    fprintf( file, "%s %d %s %f\n", "<Mixture>", m, "<Weight>", state->weight[m] );
                    fprintf( file, "%s\n", "<Mean>" );

                    for ( int k = 0; k < f_vectSize; k++ )
                    {
                        fprintf( file, "%f ", mu[0] );
                        mu++;
                    }
                    fprintf( file, "\n" );

                    fprintf( file, "%s\n", "<Inverted_Deviation>" );

                    for ( int k = 0; k < f_vectSize; k++ )
                    {
                        fprintf( file, "%f ", invVar[0] );
                        invVar++;
                    }
                    fprintf( file, "\n" );

                    fprintf( file, "%s %f\n", "<LogVarVal>", state->log_var_val[m] );
                }
            }
        }

        // write the transition probability matrix
        fprintf( file, "%s\n", "<TransP>" );

        float *prob = hmm->transP;

        for ( int j = 0; j < hmm->num_states; j++ )
        {
            for ( int k = 0; k < hmm->num_states; k++ )
            {
                fprintf( file, "%f ", *prob );
                prob++;
            }
            fprintf( file, "\n" );
        }

        if ( hmm->level == 0 )
        {
            fprintf( file, "%s\n", "<EndEmbeddedHMM>" );
        }
        else
        {
            fprintf( file, "%s\n", "<EndExternalHMM>" );
        }

        hmm = &(f_hmm->u.ehmm[i]);
    }
    fclose( file );

    return true;
}
