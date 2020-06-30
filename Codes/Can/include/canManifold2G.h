#ifndef USER_CAN_H
#define USER_CAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdint.h>
#include <fstream>
#include <cstring>
#include <opencv2/opencv.hpp>

#include "constants.h"
//Canjiegouti
class Can {
public:
    Can(){};
    ~Can(){};
    int canTansfer(unsigned char data[]);
    int canReadData(unsigned char* buffer);
};
extern Can can;

//单片机端回传数据结构体
//英雄车
/*struct McuData {
    uint8_t enemy_color;      // 当前大云台yaw角度
    uint8_t big_pitch;    // 当前大云台pitch角度
    uint8_t small_yaw;    // 当前小云台yaw角度
    uint8_t small_pitch;  // 当前小云台pitch角度
    uint8_t speed;        // 子弹速度
    uint8_t env_light;        // 光照强度
    uint8_t mode;         // 大/小云台工作模式 0x00：大云台直射，小云台全自动  0x01:大云台抛射，小云台全自动
    uint8_t state;        // 前4位表示工作状态，后4位表示敌方战车颜色
};                    //前4位 0：待定，1：瞄准，2：瞄准+识别，3：瞄准+跟踪，4：全自动，5：瞄准+识别+弹道，6：瞄准+跟踪+弹道  后4位 0：蓝色，1：红色
*/
//步兵车（只发送一个云台数据即可）
struct McuData {
    uint8_t enemy_color;      // 当前大云台yaw角度
    uint8_t big_pitch;    // 当前大云台pitch角度
    uint8_t small_yaw;    // 当前小云台yaw角度
    uint8_t small_pitch;  // 当前小云台pitch角度
    uint8_t speed;        // 子弹速度
    uint8_t env_light;        // 光照强度
    uint8_t mode;         // 大/小云台工作模式
    uint8_t state;        // 前4位表示工作状态，后4位表示敌方战车颜色
}; 

// 串口接收函数
void canReceive(Can *pCan);


//mcu传输数据初始化结构体
extern McuData mcu_data;
//发送坐标点数据结构
void sendPosition(cv::Rect2d rect,cv::Mat src);

#endif