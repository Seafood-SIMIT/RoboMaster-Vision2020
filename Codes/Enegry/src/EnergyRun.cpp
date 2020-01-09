//
// Created 11.14/19.
//
#include "energy.h"
//#include "log.h"
//#include "config/setconfig.h"
//#include "options.h"

using namespace std;
using namespace cv;


//----------------------------------------------------------------------------------------------------------------------
// 此函数为能量机关模式主控制流函数，且步兵仅拥有云台摄像头
// ---------------------------------------------------------------------------------------------------------------------
void Energy::run(cv::Mat &src) {
    clearAll();
    waitKey(0);
    if (findArmors(src) < 1) 
    showArmors("armor", src);
    
}


