//
// Created by sunlin on 2020-10-11
//
#ifndef KALMAN_H_
#define KALMAN_H_

#include "opencv2/video/tracking.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <iostream>
class Kalman{
	private:
		const int stateNum=4;                                      //状态值4×1向量(x,y,△x,△y)
	    const int measureNum=2;                                    //测量值2×1向量(x,y)	
		cv::KalmanFilter *KF ;	
		cv::Mat prediction;
		cv::Mat measurement ;  
		cv::Point predict_pt ;
	public:
        Kalman(){
			this->KF=new cv::KalmanFilter(stateNum, measureNum, 0);
			this->KF->transitionMatrix = (cv::Mat_<float>(4, 4) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);  //转移矩阵A
			

			setIdentity(KF->measurementMatrix);                                             //测量矩阵H
			setIdentity(KF->processNoiseCov, cv::Scalar::all(1e-5));                            //系统噪声方差矩阵Q
			setIdentity(KF->measurementNoiseCov, cv::Scalar::all(1e-1));                        //测量噪声方差矩阵R
			setIdentity(KF->errorCovPost, cv::Scalar::all(1));                                  //后验错误估计协方差矩阵P
			//rng.fill(K input(200,300);ePost,RNG::UNIFORM,0,winHeight>winWidth?winWidth:winHeight);   //初始状态值x(0)
			randn(KF->statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));
		}
		cv::Point2d predict(cv::Point input);  //卡尔曼滤波预测函数

};
//卡尔曼预测
#endif