/*
 *===================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：facedetect.h
 * 文件摘要：本文件用于声明人脸检测算法的类
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/18
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *===================================================================================================
 */

#ifndef FACEDETECT_H
#define FACEDETECT_H

#include <QString>
#include <cv.h>

class FaceDetect
{
public:
    FaceDetect( void );
    ~FaceDetect( void );

    void DetectAndDraw( IplImage *img, double scale = 1.3 );

private:
    CvHaarClassifierCascade *fd_cascade;
    CvHaarClassifierCascade *fd_nestedCascade;
    CvMemStorage            *fd_storage;
    QString                  fd_cascadeName;
    QString                  fd_nestedCascadeName;
};

#endif // FACEDETECT_H
