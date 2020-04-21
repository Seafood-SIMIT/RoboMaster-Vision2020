
/*-----------------------------文件--------------------
*   文件名：LightBox.cpp
*   作者：  王妍璐 江培玲
*   功能：  对图片进行通道拆分和腐蚀膨胀以提取出灯条
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------
#include "ArmorFinder.h"
using namespace cv;
using namespace std;
//参数配置
/**
 * @name        AutoAiming::findArmorBoxTop
 * @author      seafood
 * @par         cv::Mat &srcImage,cv::Mat &processImage, ArmorBox &box
 * @return      bool
 * @function    找到装甲板顶层代码
 * */
bool AutoAiming::findArmorBoxTop(cv::Mat &srcImage,cv::Mat &processImage, ArmorBox &box)
{
    //-----------------------------------------------------------------------------
    //定义灯条
    LightBlobs light_blobs;
    //寻找所有的灯条
    //cout<<"[1] Find Blobs"<<endl;
    if(!findLightBolbsSJTU(srcImage,processImage,light_blobs))
    {
        //如果没找到灯条
        //cout<<"find less than 1 blobs"<<endl;
        return false;
    }
    //显示所有的灯条
    //cout<<"[2] Show Blobs"<<endl;
    //cout<<"Blobs after choose "<<light_blobs.size()<<endl;
    if (show_light_blobs && state==SEARCHING_STATE) 
    {
        drawLightBlobs(srcImage,light_blobs);
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
    if (show_armor_boxes && state==SEARCHING_STATE) 
    {
        showArmorBoxes("boxes", srcImage, armor_boxes);
    }
    //追踪模式box才会起作用
    if(state == TRACKING_STATE)
    {
        box = armor_boxes[0];
    }
    //都执行结束才返回真值
    return true;
}
/**
*@author：王妍璐 江培玲
*@name：main()
*@return:void
*@function：
*@para：box: img:
*其他要注意的地方
**/  
bool AutoAiming::stateSearchingTarget(cv::Mat &g_srcImage,cv::Mat &g_processImage)
{
    //搜索装甲板
    if(findArmorBoxTop(g_srcImage,g_processImage,box_number))
    {
        return true;
    }
    else
    {
        return false;
    }
    
        
}