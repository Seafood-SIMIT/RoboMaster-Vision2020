//
// Created by seafood on 2020-10-11
//
#ifndef _FIND_LIGHT_BLOBS_H_
#define _FIND_LIGHT_BLOBS_H_

#include "mcudata.h"
#include "constants.h"
#include "options.h"
#include <opencv2/imgproc/types_c.h>
// 此结构体包括灯条参数
// ---------------------------------------------------------------------------------------------------------------------
struct BlobPartParam 
{
	double lw_ratio_min;        //长宽比最小值
    double area_ration_min;     //轮廓面积与最小外接矩形面积之比最小
};


class LightBlob {
public:
    cv::RotatedRect rect;   //灯条位置
    double area_ratio;      //轮廓面积与最小外接矩形面积之比
    double length;          //灯条长度
    //uint8_t blob_color;      //灯条颜色

    LightBlob() = default;
    LightBlob(cv::RotatedRect &r, double ratio, uint8_t l) : rect(r), area_ratio(ratio), length(l) {}

private:
};

typedef std::vector<LightBlob> lightBlobs;      //灯条矢量  

bool findLightBolbsSJTU(cv::Mat &g_srcImage,cv::Mat &g_processImage,lightBlobs &light_blobs);   //寻找灯条
#endif