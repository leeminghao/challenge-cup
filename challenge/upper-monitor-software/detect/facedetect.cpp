/*
 *=================================================================================================
 *                    2011挑战杯 便携式HMM远程面部检测识别综合控制应用平台
 *
 * (C) Copyright 2011, 李明浩
 * All Rights Reserved
 *
 * 文件名称：facedetect.cpp
 * 文件摘要：本文件用于实现人脸检测算法的类
 *
 * 当前版本：0.1
 * 作   者：李明浩
 * 完成日期：2011/03/18
 *
 * 取代版本：0.0
 * 作   者：李明浩
 * 完成日期：2011/03/10
 *=================================================================================================
 */

#include "facedetect.h"
#include <highgui.h>
#include <QImage>

extern bool      g_hasDetectFace;
extern IplImage *g_findImage;

FaceDetect::FaceDetect( void )
{
    fd_storage           = cvCreateMemStorage( 0 );
    fd_cascadeName       = "detect/haarcascade_frontalface_alt.xml";
    fd_nestedCascadeName = "detect/haarcascade_eye_tre_eyeglasses.xml";
    fd_cascade           = (CvHaarClassifierCascade *)cvLoad(
                                fd_cascadeName.toStdString().c_str(),
                                NULL,
                                NULL,
                                NULL
                                );
    fd_nestedCascade     = (CvHaarClassifierCascade *)cvLoad(
                                fd_nestedCascadeName.toStdString().c_str(),
                                NULL,
                                NULL,
                                NULL
                                );
}

FaceDetect::~FaceDetect( void )
{
    cvReleaseMemStorage( &fd_storage );
}

void
FaceDetect::DetectAndDraw( IplImage *img, double scale /* = 1.3 */ )
{
    CvScalar colors[] =
    {
        CV_RGB(   0.0,   0.0, 255.0 ), CV_RGB(   0.0, 128.0, 255.0 ),
        CV_RGB(   0.0, 255.0, 255.0 ), CV_RGB(   0.0, 255.0,   0.0 ),
        CV_RGB( 255.0, 128.0,   0.0 ), CV_RGB( 255.0, 255.0,   0.0 ),
        CV_RGB( 255.0,   0.0,   0.0 ), CV_RGB( 255.0,   0.0, 255.0 )
    };
    IplImage *gray = cvCreateImage(
        cvSize( img->width, img->height ),
        8,
        1
        );

    cvCvtColor( img, gray, CV_BGR2GRAY );            // 将图像转化为灰度图像
    cvEqualizeHist( gray, gray );
    cvClearMemStorage( fd_storage );
    if ( fd_cascade != NULL )
    {
        CvSeq *faces = cvHaarDetectObjects(
            gray,
            fd_cascade,
            fd_storage,
            1.1,
            2,
            0,
            cvSize( 30, 30 )
            );
        // 逐个取出人脸矩形区域
        for ( int i = 0; i < ((faces != NULL) ? faces->total : 0); i++ )
        {
            CvRect   *r = (CvRect *)cvGetSeqElem( faces, i );
            CvMat     smallImgRoi;
            CvSeq    *nestedObjects;
            CvPoint   center;
            CvScalar  color = colors[i % 8];
            int       radius;

            cvSetImageROI( img, *r );
            g_findImage = cvCreateImage( cvSize( r->width, r->height ), 8, 3 );
            cvCopy( img, g_findImage );
            cvResetImageROI( img );

            center.x = cvRound( (r->x + r->width * 0.5) );
            center.y = cvRound( (r->y + r->height * 0.5) );
            radius   = cvRound( (r->width + r->height) * 0.25 );
            cvCircle( img, center, radius, color, 3, 8, 0 );

            g_hasDetectFace = true;

            if ( fd_nestedCascade == NULL )
            {
                continue;
            }
            cvGetSubRect( gray, &smallImgRoi, *r );
            nestedObjects = cvHaarDetectObjects(
                &smallImgRoi,
                fd_nestedCascade,
                fd_storage,
                1.1,
                2,
                0,
                cvSize( 0, 0 )
                );
            for ( int j = 0; j < ((nestedObjects != NULL) ? nestedObjects->total : 0); j++ )
            {
                CvRect *nr = (CvRect *)cvGetSeqElem( nestedObjects, j );

                center.x = cvRound( (r->x + nr->x + nr->width * 0.5) * scale);
                center.y = cvRound( (r->y + nr->y + nr->height * 0.5) * scale);
                radius   = cvRound( (nr->width + nr->height) * 0.25 * scale );
                cvCircle( img, center, radius, color, 3, 8, 0 );
            }
        }
    }
    cvReleaseImage( &gray );
}
