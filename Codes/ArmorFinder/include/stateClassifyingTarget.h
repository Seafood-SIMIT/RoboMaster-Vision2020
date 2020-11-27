//
// Created by seafood on 2020-09-28
//
#ifndef _STATE_CLASSIFYING_TARGET_H_
#define _STATE_CLASSIFYING_TARGET_H_

#include "ArmorBox.h"
#include "GetFeature.h"

class StateClassifyingTarget{
public:
    ArmorBox run(cv::Mat &g_srcImage,cv::Mat &g_processImage, armorBoxes armorboxes);      //运行函数
private:
    ArmorBox numberClassifyRoi(cv::Mat &g_srcImage,cv::Mat &g_processImage, armorBoxes armorboxes);                //数字分类器
    double getarmorPixelS();        // 获取灯条长度和间距的比例
    //double getBlobsDistance();      // 获取两个灯条中心点的间距
};
#endif