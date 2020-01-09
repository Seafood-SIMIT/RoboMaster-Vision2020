//
// Created by xixiliadorabarry on 1/24/19.
//
#ifndef ENERGY_H
#define ENERGY_H
#include <iostream>
#include <vector>
#include <queue>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "param_struct_define.h"
//#include <constants.h>
//#include <stdio.h>
//#include <time.h>
//#include <systime.h>
//#include "energy/constant.h"
//#include "energy/param_struct_define.h"
//#include "serial.h"
//#include "additions.h"
//#include "options.h"

using std::vector;

class Energy {
public:
    Energy(uint8_t &color);//构造函数，参数为敌方颜色
    ~Energy();//默认析构函数

    //bool is_big;//大符模式为true
    //bool is_small;//小符模式为true

    void run(cv::Mat &src);

    //Serial &serial;//串口
    void setEnergyInit();//设置能量机关初始化
   // void sendEnergy();//发送能量机关数据
   // void sendTarget(Serial& serial, float x, float y, float z, uint16_t u);//发送数据


private:
    EnergyPartParam energy_part_param_;//能量机关的参数设置

    //bool is_guessing;//当前处于发弹到新目标出现的过程，则为true，此时猜测下一个目标
    //bool is_predicting;//当前处于新目标出现到发弹的过程，则为true，此时正常击打
    //bool energy_mode_init;//大小符状态判断
    //bool energy_rotation_init;//若仍在判断风车旋转方向，则为true
    // bool start_guess;//进入猜测状态的标志
    //bool change_target;//目标切换的标志

    uint8_t &ally_color;//我方颜色

    //int curr_fps;//帧率
    //int send_cnt;//向主控板发送的数据总次数
    //int fans_cnt;//扇叶个数
    //int last_fans_cnt;//上一帧的扇叶个数
    //int guess_devide;//刚进入猜测状态时，猜测目标点在极坐标中的分区
    //int energy_rotation_direction;//风车旋转方向
    //int clockwise_rotation_init_cnt;//装甲板顺时针旋转次数
    //int anticlockwise_rotation_init_cnt;//装甲板逆时针旋转次数
    //int last_mode;//上一帧的能量机关状态
    //int manual_delta_x, manual_delta_y;//手动微调量
    //int extra_delta_x, extra_delta_y;//在风车运动到最高点附近的额外补偿量

    //float target_polar_angle;//待击打装甲板的极坐标角度
    //float last_target_polar_angle_judge_change;//上一帧待击打装甲板的极坐标角度（用于判断目标切换）
    //float last_target_polar_angle_judge_rotation;//上一帧待击打装甲板的极坐标角度（用于判断旋向）
    //float guess_polar_angle;//猜测的下一个目标装甲板极坐标角度
    //float last_base_angle;//上一帧的各扇叶在0区（0°~72°）的基础角度
    //float predict_rad;//预测提前角
    //float predict_rad_norm;//预测提前角的绝对值
    //float attack_distance;//步兵与风车平面距离
    //float center_delta_yaw, center_delta_pitch;//对心时相差的角度
    //float yaw_rotation, pitch_rotation;//云台yaw轴和pitch轴应该转到的角度
    //float shoot;//给主控板的指令，1表示跟随，2表示发射，3表示目标切换,4表示猜测模式
    //float last_yaw, last_pitch;//PID中微分项
    //float sum_yaw, sum_pitch;//yaw和pitch的累计误差，即PID中积分项

    //systime time_start_guess;//进入猜测模式的时间

    //cv::RotatedRect centerR;//风车中心字母R的可能候选区
    //cv::RotatedRect flow_strip;//图像中所有流动条（理论上只有一个）
    //cv::RotatedRect flow_strip_fan;//图像中所有流动条所在扇叶（理论上只有一个）
    //cv::RotatedRect center_ROI;//风车中心候选区
    //cv::RotatedRect target_armor;//目标装甲板（理论上仅一个）

    //cv::Point circle_center_point;//风车圆心坐标
    //cv::Point target_point;//目标装甲板中心坐标
    //cv::Point guess_point;
    //cv::Point predict_point;//预测的击打点坐标

    cv::Mat src_blue, src_red, src_green;//通道分离中的三个图像通道
    std::vector<cv::RotatedRect> armors;//图像中所有可能装甲板（可能存在误识别)
    std::vector<cv::RotatedRect> armors_external;//用于框出最外部轮廓，判断扇叶

    void initEnergy();//能量机关初始化

    void initEnergyPartParam();//能量机关参数初始化

    void clearAll();//清空各vector

    void initImage(cv::Mat &src);//图像预处理


    int findArmors(const cv::Mat &src);//寻找图中所有装甲板

    bool isValidArmorContour(const vector<cv::Point> &armor_contour);//装甲板矩形要求

    bool isValidExtArmorContour(const vector<cv::Point> &armor_contour);//能量机关装甲板外廓矩形要求

    void showArmors(std::string windows_name, const cv::Mat &src);//显示装甲板
 
    double pointDistance(cv::Point point_1, cv::Point point_2);//计算两点距离

    double nonZeroRateOfRotateRect(cv::Mat &bin, const cv::RotatedRect &rotatedRect);//计算旋转矩形内亮点占比

    void imagePreprocess(cv::Mat &src);//图像通道分离

    void ArmorStruct(cv::Mat &src);//腐蚀和膨胀
 
};
int energy(cv::Mat &src);
#endif //ENERGY_H

