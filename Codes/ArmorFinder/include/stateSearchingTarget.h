//
// Created by seafood on 2020-09-28
//
#ifndef _STATE_SEARCHING_TARGET_H_
#define _STATE_SEARCHING_TARGET_H_

#include "ArmorBox.h"

class StateSearchingTarget
{
public:
    armorBoxes armor_boxes;     //目标的装甲板
    bool run(cv::Mat &srcImage,cv::Mat &processImage);      //运行函数
private:
    lightBlobs light_blobs;
    BlobPartParam blob_parament;        //灯条相关参数
    cv::Mat color_channel;              //颜色通道
    cv::Mat processImage_bin;           //源图
    std::vector<cv::Mat> channels;               //通道数 
    int light_threshold;                //设定亮图片阈值
};
#endif