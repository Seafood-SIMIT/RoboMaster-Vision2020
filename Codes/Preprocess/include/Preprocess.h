#ifndef __PREPROCESS__H
#define __PREPROCESS__H


#include <opencv2/core/core.hpp>

#include "options.h"            //参数选择
#include "constants.h"          //常数设定
#include "canManifold2G.h"      //CAN数据
#include "mcudata.h"

//图像预处理
class Preprocess
{
public:
    void run(cv::Mat &g_processImage);     //run函数，运行
private:
    void setParamInit();    //设置参数
    void clearWhiteLight(cv::Mat &g_processImage); //  消除日光影响
};
#endif