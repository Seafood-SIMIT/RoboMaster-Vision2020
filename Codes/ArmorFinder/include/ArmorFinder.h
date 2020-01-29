//
// Created by xinyang on 19-3-27.
//
#ifndef _ARMOR_FINDER_H_
#define _ARMOR_FINDER_H_

#include <opencv2/opencv.hpp>
#include <string>
#include <Eigen/Core>
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
class ArmorBox{
public:
    cv::Rect2d rect;
    LightBlobs light_blobs;
    uint8_t box_color;
    int id;
    ArmorBox(const cv::Rect &pos, const LightBlobs &blobs, uint8_t color):rect(pos),light_blobs(blobs),box_color(color){};

    cv::Point2f getCenter() const; // 获取装甲板中心
    double getBlobsDistance() const; // 获取两个灯条中心间距
    double lengthDistanceRatio() const; // 获取灯条中心距和灯条长度的比值
    double getBoxDistance() const; // 获取装甲板到摄像头的距离

    bool operator<(const ArmorBox &box) const; // 装甲板优先级比较
};
typedef std::vector<ArmorBox> ArmorBoxes;
#define BLOB_RED 1
#define BLOB_BLUE 0
#define ENEMY_RED 1
#define ENEMY_BLUE 0
bool findLightBolbsSJTU(cv::Mat &input_img);
bool findLightBolbsCSDN(cv::Mat &input_img);
void showLightBlobs(const cv::Mat &input_image,std::string windows_name,const LightBlobs &light_blobs);
bool matchArmorBoxes(const cv::Mat &src, const LightBlobs &light_blobs, ArmorBoxes &armor_boxes);
void showArmorBoxes(std::string windows_name, const cv::Mat &src, const ArmorBoxes &armor_boxes);
#endif /* _ARMOR_FINDER_H_ */
