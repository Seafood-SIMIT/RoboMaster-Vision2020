
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
bool show_armor_box = true;
bool show_armor_boxes = true;
bool show_light_blobs = true;
bool show_origin = false;
bool run_with_camera = false;
bool save_video = false;
bool wait_uart = false;
bool save_labelled_boxes = false;
bool show_process = false;
bool show_energy = false;
bool save_mark = false;
bool show_info = false;
bool run_by_frame = false;

bool AutoAiming::findArmorBoxTop(cv::Mat &srcImage,cv::Mat &processImage, ArmorBox &box)
{
    LightBlobs light_blobs;
    //寻找所有的灯条
    if(!findLightBolbsSJTU(srcImage,processImage,light_blobs))
    {
        //cout<<"find less than 1 blobs"<<endl;
        return false;
    }
    //显示所有的灯条
    //cout<<"Blobs after choose "<<light_blobs.size()<<endl;
    /*if (show_light_blobs && state==SEARCHING_STATE) 
    {
        drawLightBlobs(srcImage,light_blobs);
    }*/
    // 对灯条进行匹配得出装甲板候选区
    if(!matchArmorBoxes(processImage,light_blobs,armor_boxes))
    {
        //cout<<"No armorbox"<<endl;
        return false;
        //显示所有装甲板
    }
    //cout<<"armorbox detected"<<endl;
    //if (show_armor_boxes && state==TRACKING_STATE) 
    if (show_armor_boxes ) 
    {
        showArmorBoxes("boxes", srcImage, armor_boxes);
    }
    box = armor_boxes[0];
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
    if(findArmorBoxTop(g_srcImage,g_processImage,box_number))
    {
        return true;
    }
    else
    {
        return false;
    }
    
        
}