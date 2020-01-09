//
// Created by xixiliadorabarry on 1/24/19.
//
#include "energy.h"
//#include "log.h"

using namespace cv;
using std::cout;
using std::endl;
using std::vector;

//----------------------------------------------------------------------------------------------------------------------
// 此函数用于寻找图像内所有的大风车装甲板模块
// ---------------------------------------------------------------------------------------------------------------------
int Energy::findArmors(const cv::Mat &src) {
    if (src.empty()) {
        cout << "empty!" << endl;
        return 0;
    }
    static Mat src_bin;
    src_bin = src.clone();
    if (src.type() == CV_8UC3) {
        cvtColor(src_bin, src_bin, CV_BGR2GRAY);//若读取三通道视频文件，需转换为单通道
    }
    std::vector<vector<Point> > armor_contours;
    std::vector<vector<Point> > armor_contours_external;//用总轮廓减去外轮廓，只保留内轮廓，除去流动条的影响。

    ArmorStruct(src_bin);//图像膨胀，防止图像断开并更方便寻找
    findContours(src_bin, armor_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    //imshow("armor struct装甲板", src_bin);
    //waitKey(0);
    findContours(src_bin, armor_contours_external, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (int i = 0; i < armor_contours_external.size(); i++)//去除外轮廓
    {
        unsigned long external_contour_size = armor_contours_external[i].size();
        for (int j = 0; j < armor_contours.size(); j++) {
            unsigned long all_size = armor_contours[j].size();
            if (external_contour_size == all_size) {
                swap(armor_contours[j], armor_contours[armor_contours.size() - 1]);
                armor_contours.pop_back();
                break;
            }
        }
    }

    for (auto &armor_contour_external : armor_contours_external) {       //添加部分，可以框出外部轮廓，用于截取ROI
     if (!isValidExtArmorContour(armor_contour_external)) {
          continue;
       }
        armors_external.emplace_back(cv::minAreaRect(armor_contour_external));
    }
   


    for (auto &armor_contour : armor_contours) {
       if (!isValidArmorContour(armor_contour)) {
          continue;
       }
        armors.emplace_back(cv::minAreaRect(armor_contour));
    }

   // if (show_info) {
//        if (armors.size() < 1)cout << "no armors!" << endl;
   // }

    return static_cast<int>(armors.size());
}

