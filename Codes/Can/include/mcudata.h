//
// Created by seafood on 2020-10-11
//
#ifndef _MCUDATA_H_
#define _MCUDATA_H_

#include "canManifold2G.h"
#include <stdint.h>
#include <stdlib.h>

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

static McuData mcu_data;       //与电控沟通信息
                        //初始化及接口函数在mcudata中，其他均为读取

            
void mcuDataReceive(Can *pCan);        //can总线接收数据线程
#endif