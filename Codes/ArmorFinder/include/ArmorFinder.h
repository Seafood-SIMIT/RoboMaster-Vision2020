//
// Created by xinyang on 19-3-27.
//
#ifndef _ARMOR_FINDER_H_
#define _ARMOR_FINDER_H_

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <Eigen/Core>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/tracking.hpp>

//----------------------------------------------------------------------------------------------------------------------
// 此结构体包括灯条参数
// ---------------------------------------------------------------------------------------------------------------------
struct BlobPartParam 
{
	int RED_GRAY_THRESH;
	int BLUE_GRAY_THRESH;
	int SPLIT_GRAY_THRESH;
    long BLOB_CONTOUR_AREA_MAX;//装甲板面积最大值
	long BLOB_CONTOUR_AREA_MIN;//装甲板面积最小值
	long BLOB_CONTOUR_LENGTH_MIN;//装甲板长边长度最小值
	long BLOB_CONTOUR_WIDTH_MIN;//装甲板长边长度最大值
	long BLOB_CONTOUR_LENGTH_MAX;//装甲板宽边长度最小值
	long BLOB_CONTOUR_WIDTH_MAX;//装甲板宽边长度最大值
	float BLOB_CONTOUR_HW_RATIO_MAX;//装甲板长宽比最大值
	float BLOB_CONTOUR_HW_RATIO_MIN;//装甲板长宽比最小值
	float BLOB_CONTOUR_AREA_RATIO_MIN;//装甲板轮廓占旋转矩形面积比最小值
};
/******************* 灯条类定义 ***********************/
class LightBlob {
public:
    cv::RotatedRect rect;   //灯条位置
    double area_ratio;
    double length;          //灯条长度
    uint8_t blob_color;      //灯条颜色

    LightBlob(cv::RotatedRect &r, double ratio, uint8_t color) : rect(r), area_ratio(ratio), blob_color(color) {
        length = max(rect.size.height, rect.size.width);
    };
    double max(double x,double y){
        return x>y?x:y;
    }
    LightBlob() = default;
};
typedef std::vector<LightBlob> LightBlobs;

/******************* 装甲板类定义　**********************/
class ArmorBox{
public:
    cv::Rect2d rect;
    LightBlobs light_blobs;
    uint8_t box_color;
    int id;
    //ArmorBox(const cv::Rect &pos, const LightBlobs &blobs, uint8_t color):rect(pos),light_blobs(blobs),box_color(color){}.
    explicit ArmorBox(const cv::Rect &pos=cv::Rect2d(), const LightBlobs &blobs=LightBlobs(), uint8_t color=0);

    /*cv::Point2f getCenter() const; // 获取装甲板中心
    double getBlobsDistance() const; // 获取两个灯条中心间距
    double lengthDistanceRatio() const; // 获取灯条中心距和灯条长度的比值
    double getBoxDistance() const; // 获取装甲板到摄像头的距离*/

};
typedef std::vector<ArmorBox> ArmorBoxes;
//
/********************* 自瞄类定义 **********************/
class AutoAiming{
private:       
    cv::Ptr<cv::Tracker> tracker;                       // tracker对象实例
    ArmorBox target_box, last_box;  //目标装甲板 上一个装甲板
    int tracking_cnt;
    int contour_area;                                   // 装甲区域亮点个数，用于数字识别未启用时判断是否跟丢（已弃用）

    //函数
    bool stateTrackingTarget(cv::Mat &src);
    bool findArmorBoxTop(cv::Mat &g_srcImage,cv::Mat &g_processImage,ArmorBox &target_box);
    bool stateSearchingTarget(cv::Mat &g_srcImage,cv::Mat &g_processImage);
    bool stateTrackingTarget(cv::Mat &g_srcImage,cv::Mat &g_processImage);
    bool findLightBolbsSJTU(cv::Mat &g_srcImage,cv::Mat &g_processImage,LightBlobs &light_blobs);
    bool matchArmorBoxes(const cv::Mat &src, const LightBlobs &light_blobs, ArmorBoxes &armor_boxes);
    void drawLightBlobs(cv::Mat &g_srcImage, const LightBlobs &blobs);
    void showArmorBoxes(std::string windows_name, const cv::Mat &src, const ArmorBoxes &armor_boxes);
    void showArmorBox(std::string windows_name, const cv::Mat &g_srcImage, const cv::Rect2d &armor_box);
public:
    //战车状态定义
    typedef enum{
        SEARCHING_STATE, TRACKING_STATE, STANDBY_STATE
    } State;
    State state;

    void run(cv::Mat &g_srcImage,cv::Mat &g_processImage);
};
                      // 目标装甲板
#define BLOB_RED 1
#define BLOB_BLUE 0
#define ENEMY_RED 1
#define ENEMY_BLUE 0

//sunlin out

#endif /* _ARMOR_FINDER_H_ */
