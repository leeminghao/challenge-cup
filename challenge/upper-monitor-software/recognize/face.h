/*
 *===================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：face.h
 * 文件摘要：本文件用于声明人脸的类, 利用人脸图片信息进行训练。
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

#ifndef FACE_H
#define FACE_H

#include <cvaux.h>    // OpenCV库头文件
#include <cv.h>       // OpenCV库头文件
#include <highgui.h>  // OpenCV库头文件
#include <list>

#define STATE_NUM    32
#define MIXTURE_NUM 128
#define TEMP_SIZE   128
#define MAX_PATH    128

typedef struct hmm_params
{
    int    hp_state[STATE_NUM];        // EHMM的超态数目
    int    hp_mixture[MIXTURE_NUM];    // 高斯混合因子的数目
    int    hp_max;
    CvSize hp_dct;                     // DCT窗口尺寸
    CvSize hp_obs;
    CvSize hp_delta;

} HMM_PARAMS;

class Face
{
public:
     Face( void );
    ~Face( void );

    bool release( void );

    CvEHMM            *getEHMM( void );
    std::list<char *>  getImageList( void );

    void addImageToList( char *imagename );
    bool delImageFromList( int pos );
    void delAllImageFromList( void );
    void trainHMM( HMM_PARAMS &params );
    bool isTrained( void );

    bool load( char *fileName );
    bool save( char *fileName );

private:
    CvEHMM            *f_hmm;
    std::list<char *>  f_imageList;
    bool               f_trained;
    int                f_vectSize;
};

#endif // FACE_H
