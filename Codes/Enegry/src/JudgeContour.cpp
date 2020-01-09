//
// Created by sun on 19-7-13.
//

#include "energy.h"

using namespace std;
using namespace cv;


//----------------------------------------------------------------------------------------------------------------------
// 此函数用于判断找到的矩形候选区是否为装甲板
// ---------------------------------------------------------------------------------------------------------------------
bool Energy::isValidArmorContour(const vector<cv::Point> &armor_contour) {
    double cur_contour_area = contourArea(armor_contour);
    if (cur_contour_area > energy_part_param_.ARMOR_CONTOUR_AREA_MAX ||
        cur_contour_area < energy_part_param_.ARMOR_CONTOUR_AREA_MIN) {
        //cout<<cur_contour_area<<" "<<energy_fan_param_.CONTOUR_AREA_MIN<<" "<<energy_fan_param_.CONTOUR_AREA_MAX<<endl;
        //cout<<"area fail."<<endl;
        //return false;
        //选区面积大小不合适
    }
    RotatedRect cur_rect = minAreaRect(armor_contour);
    Size2f cur_size = cur_rect.size;
    float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//将矩形的长边设置为长
    float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//将矩形的短边设置为宽
    if (length < energy_part_param_.ARMOR_CONTOUR_LENGTH_MIN || width < energy_part_param_.ARMOR_CONTOUR_WIDTH_MIN ||
        length > energy_part_param_.ARMOR_CONTOUR_LENGTH_MAX || width > energy_part_param_.ARMOR_CONTOUR_WIDTH_MAX) {
        //cout<<"length width fail."<<endl;
//        cout << "length: " << length << '\t' << "width: " << width << '\t' << cur_rect.center << endl;
        //return false;
        //矩形边长不合适
    }

    float length_width_ratio = length / width;//计算矩形长宽比
    if (length_width_ratio > energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MAX ||
        length_width_ratio < energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MIN) {
        //cout<<"length width ratio fail."<<endl;
//        cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
        return false;
        //长宽比不合适
    }
    if (cur_contour_area / cur_size.area() < energy_part_param_.ARMOR_CONTOUR_AREA_RATIO_MIN) {
//        cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
        //return false;//轮廓对矩形的面积占有率不合适
    }
    return true;
}

bool Energy::isValidExtArmorContour(const vector<cv::Point> &armor_contour_external) {
    double cur_contour_area2 = contourArea(armor_contour_external);
    //cout<<cur_contour_area2<<endl;
    if (cur_contour_area2 > 3000) {
        return true;
    }
}