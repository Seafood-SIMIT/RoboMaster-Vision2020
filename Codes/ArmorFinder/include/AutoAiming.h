//
// Created by seafood on 2020-09-28
//
#ifndef _AUTO_AIMING_H_
#define _AUTO_AIMING_H_
//include
#include "stateClassifyingTarget.h"
#include "stateTrackingTarget.h"
#include "CameraInit.h"
#include <opencv2/tracking.hpp>
//战车状态定义：搜索状态、分类、追踪、待定
typedef enum{
    SEARCHING_STATE, CLASSIFYING_STATE, TRACKING_STATE, STANDBY_STATE
} State;
/********************* 自瞄类定义 **********************/
class AutoAiming{
public:
    AutoAiming() = default;
    //声明状态
    State state; 
    void run(cv::Mat g_srcImage,cv::Mat g_processImage);

    AutoAiming(cv::Mat &srcImage, cv::Mat &processImage):autoaiming_srcImage(srcImage), autoaiming_processImage(processImage){}
private:
    cv::Mat autoaiming_srcImage,autoaiming_processImage;
    armorBoxes armor_boxes;
    StateSearchingTarget statesearchingtarget;      //搜寻状态对象
    StateClassifyingTarget stateclassifyingtarget;  //分类状态对象
    StateTrackingTarget statetrackingtarget;       //追踪状态对象
    void searchingProm(cv::Mat g_srcImage,cv::Mat g_processImage);   //搜索程序
    void classifyingProm(cv::Mat g_srcImage,cv::Mat g_processImage); //分类程序
    void trackingProm(cv::Mat g_srcImage,cv::Mat g_processImage);     //跟踪程序
    //声明卡尔曼滤波器
    Kalman *kalman_filter;
    cv::Ptr<cv::Tracker> tracker;                       // tracker对象实例
    ArmorBox target_box, last_box;  //目标装甲板 上一个装甲板
    ArmorBox box_number;                   //数字识别中的目标
    
    int tracking_cnt;
};
#endif /* _AUTO_AIMING_H_ */
/*
//函数
    
    bool matchArmorBoxes(const cv::Mat &src, const LightBlobs &light_blobs, ArmorBoxes &armor_boxes);
    void drawLightBlobs(cv::Mat &g_srcImage, const LightBlobs &blobs);
    void showArmorBoxes(std::string windows_name, const cv::Mat &src, const ArmorBoxes &armor_boxes);
    void showArmorBox(std::string windows_name, const cv::Mat &g_srcImage, const cv::Rect2d &armor_box);
*/