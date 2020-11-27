
/*-----------------------------文件--------------------
*   文件名：    stateSearchingTarget.cpp
*   作者：      seafood
*   功能：      搜寻状态主代码
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------
#include "stateSearchingTarget.h"
using namespace cv;
using namespace std;

/**
 * @name        findArmorBoxTop
 * @author      seafood
 * @par         cv::Mat &srcImage,cv::Mat &processImage, ArmorBox &box
 * @return      bool
 * @function    找到装甲板顶层代码
 * */
bool StateSearchingTarget::run(cv::Mat &srcImage,cv::Mat &processImage)
{
    //寻找所有的灯条
    //cout<<"[1] Find Blobs"<<endl;
    if(!findLightBolbsSJTU(srcImage,processImage,light_blobs))
    {
        //如果没找到灯条
        //cout<<"find less than 1 blobs"<<endl;
        return false;
    }
    //------------------------------------------------------------------------------
    // 对灯条进行匹配得出装甲板候选区
    //cout<<"[3] Match Armors"<<endl;
    if(!matchArmorBoxes(processImage,light_blobs,armor_boxes))
    {
        //cout<<"No armorbox"<<endl;
        return false;
        //显示所有装甲板
    }
    //cout<<"[4] Show Armors"<<endl;
    //cout<<"armorbox detected"<<endl;
    if (show_armor_boxes )//&& state==SEARCHING_STATE) 
    {
        showArmorBoxes("boxes", srcImage, armor_boxes);
    }
    //这里增加大小排序或者位置排序.
    //追踪模式box才会起作用
    //box = armor_boxes[0];
    //都执行结束才返回真值
    return true;
}

