#ifndef __PREPROCESS__H
#define __PREPROCESS__H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "options.h"            //参数选择
#include "constants.h"          //常数设定
#include "canManifold2G.h"      //CAN数据
//---------------------------------命名空间----------------------
using namespace std;
using namespace cv;
//图像预处理
class Preprocess
{
public:
    void run(Mat &g_processImage);     //run函数，运行
private:
    void setParamInit();    //设置参数
    void clearWhiteLight(Mat &g_processImage); //  消除日光影响


};
#endif