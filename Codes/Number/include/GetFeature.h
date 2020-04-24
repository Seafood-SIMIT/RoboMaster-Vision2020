//
// Created by xinyang on 19-3-27.
//
#ifndef _GET_FEATURE_H_
#define _GET_FEATURE_H_

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

extern float yangben_Feature[24][25];       //样本特征

void getFeature(cv::Mat m,float a[25]);
float ouDistance(float a[25], float b[25]);
int getResultNumber(cv::Mat roi_armor_ostu);
int on_showImage_triggered(cv::Mat roi_armor_ostu);
#endif /* _ARMOR_FINDER_H_ */
