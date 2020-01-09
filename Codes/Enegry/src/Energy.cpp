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
// 此函数为能量机关构造函数，只要程序不重启就不会重新构造
// ---------------------------------------------------------------------------------------------------------------------
const int SRC_WIDTH = 320;
const int SRC_HEIGHT = 240;

Energy::Energy( uint8_t &color) : ally_color(color),
                                  src_blue(SRC_HEIGHT, SRC_WIDTH, CV_8UC1),
                                  src_red(SRC_HEIGHT, SRC_WIDTH, CV_8UC1) {
    //initEnergy();
    armors.clear();
    energy_part_param_.RED_GRAY_THRESH = 180;//game
    energy_part_param_.BLUE_GRAY_THRESH = 100;//game
    energy_part_param_.SPLIT_GRAY_THRESH = 180;
    energy_part_param_.ARMOR_CONTOUR_AREA_MAX = 500;
    energy_part_param_.ARMOR_CONTOUR_AREA_MIN = 180;
    energy_part_param_.ARMOR_CONTOUR_LENGTH_MIN = 10;
    energy_part_param_.ARMOR_CONTOUR_LENGTH_MAX = 50;
    energy_part_param_.ARMOR_CONTOUR_WIDTH_MIN = 0;
    energy_part_param_.ARMOR_CONTOUR_WIDTH_MAX = 30;
    energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MAX = 3;
    energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MIN = 1;
    //initEnergyPartParam();

    //is_big = false;
    //is_small = false;
}


//----------------------------------------------------------------------------------------------------------------------
// 此函数为能量机关析构函数，设置为默认
// ---------------------------------------------------------------------------------------------------------------------
Energy::~Energy() = default;


//----------------------------------------------------------------------------------------------------------------------
// 此函数为大能量机关再初始化函数
// ---------------------------------------------------------------------------------------------------------------------
/*void Energy::setEnergyInit() {
    initEnergy();
    initEnergyPartParam();

    FILE *fp = fopen(PROJECT_DIR"/Mark/delta.txt", "r");
    if (fp) {
        fscanf(fp, "delta_x: %d, delta_y: %d", &manual_delta_x, &manual_delta_y);
        fclose(fp);
    }*/

