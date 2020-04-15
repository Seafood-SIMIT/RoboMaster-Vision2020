/*********************************************************
*   文件名： kalmanfilter.cpp
*   作者：  seafood
*   内容：  使用kalman滤波器对装甲板进行预测
*     
*
**********************************************************/
#include "kalmanfilter.h"

using namespace cv;
using namespace std;

//初始位置
Point armor_first_Positin= Point(1,1);

/**
 * @name kalmanPrediction()
 * @author     Seafood
 * @par         null
 * @func        卡尔曼预测装甲板位置
 * */
int kalmanPrediction()
{
    RNG rng;
    //kalman初始化
    const int stateNum = 4;         //  状态值4x1向量(x,y,delta_x,delta_y)
    const int measureNum = 2;       //测量值2x1向量
    KalmanFilter KF(stateNum, measureNum, 0);

    KF.transitionMatrix = (Mat_<float>(4, 4) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);  //转移矩阵A
    setIdentity(KF.measurementMatrix);                                              //测量矩阵H
    setIdentity(KF.processNoiseCov, Scalar::all(1e-5));                             //系统噪声方差矩阵Q
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));                        //测量噪声方差矩阵R
    setIdentity(KF.errorCovPost, Scalar::all(1));                                  //后验错误估计协方差矩阵P
    //rng.fill(KF.statePost,RNG::UNIFORM,0,winHeight>winWidth?winWidth:winHeight);   //初始状态值x(0)
	Mat measurement = Mat::zeros(measureNum, 1, CV_32F);                           //初始测量值x'(0)，因为后面要更新这个值，所以必须先定义
    
}
