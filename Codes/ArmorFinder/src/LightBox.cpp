
/*-----------------------------文件--------------------
*   文件名：LightBox.cpp
*   作者：  王妍璐 江培玲
*   功能：  对图片进行通道拆分和腐蚀膨胀以提取出灯条
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------
#include "ArmorFinder.h"
using namespace cv;                           /*之后程序中使用cv和std命名空间的资源时不用加前缀*/
using namespace std;

/**
*@author：王妍璐 江培玲
*@name：main()
*@return:void
*@function：
*@para：box: img:
*其他要注意的地方
**/  
int lightBox(Mat image)
{
    findLightBolbsSJTU(image);
    return 1;
}