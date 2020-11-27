//
// Created by xinyang on 19-3-27.
//
#ifndef _GET_FEATURE_H_
#define _GET_FEATURE_H_

#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <string>

static float yangben_Feature[24][25];       //样本特征

int getResultNumber(cv::Mat roi_armor_ostu);     //数字识别

void getYangbenFeatures();     //获取样本特征
#endif /* _GET_FEATURE_H_ */
