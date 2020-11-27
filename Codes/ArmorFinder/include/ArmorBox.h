//
// Created by seafood on 2020-09-28
//
#ifndef _ARMOR_BOX_H_
#define _ARMOR_BOX_H_

#include "FindLightBlobs.h"
#include "constants.h"
#include <Eigen/Dense>


bool isCoupleLight(const LightBlob &light_blob_i, const LightBlob &light_blob_j);     //是成对的灯条
static bool angelJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j);     //判断两个灯条的角度差
static bool heightJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j);       //判断两个灯条的高度差

/******************* 装甲板类定义　**********************/
class ArmorBox{
public:
    ArmorBox() = default;
    cv::Rect2d rect;        //矩阵
    lightBlobs light_blobs; //装甲板的灯条
    int id;                 //装甲板id
    //构造函数
    ArmorBox(const cv::Rect &pos, const lightBlobs &blobs, uint8_t id_input):rect(pos),light_blobs(blobs),id(id_input){}
    double getarmorPixelS() const;       //获得数字ROI的面积
    static void showArmorBox(std::string windows_name, const cv::Mat &g_srcImage, const cv::Rect2d &armor_box);            //显示装甲板
private:
    
    cv::Point2f getCenter() const;      // 获取装甲板中心
    double getBlobsDistance() const;    // 获取两个灯条中心间距
    
    // 获取两个灯条中心点的间距
    //double getBlobsDistance();
};
//装甲板矢量
typedef std::vector<ArmorBox> armorBoxes;

bool matchArmorBoxes(const cv::Mat &src, const lightBlobs &light_blobs, armorBoxes &armor_boxes);   //匹配装甲板
void showArmorBoxes(std::string windows_name, const cv::Mat &g_srcImage, const armorBoxes &armor_boxes);    //显示装甲板
#endif /* _ARMOR_BOX_H_ */