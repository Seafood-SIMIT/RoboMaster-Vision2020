//
// Created by xinyang on 19-7-8.
//

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define PI (3.14159265459)

//#define ALLY_BLUE ENEMY_RED
//#define ALLY_RED  ENEMY_BLUE
//系统运行参数
#define SOURCE_CAMERA    1      //信号源为摄像机
#define SOURCE_VIDEO    0       //信号源为视频源
#define CAMERA_INIT_SUCCESS 1   //相机初始化成功
#define CAMERA_INIT_FAIL    0   //相机初始化失败
//相机参数
#define MAX_IMAGE_DATA_SIZE   (1920*1200*3)     //图像最大大小
#define BIG_ENERGY_STATE   'b'
#define SMALL_ENERGY_STATE 's'
#define ARMOR_STATE        'a'
//定义敌方颜色
#define BLOB_RED 1
#define BLOB_BLUE 0
#define ENEMY_RED 1
#define ENEMY_BLUE 0

//#define FOCUS_PIXAL_8MM  (1488)
//#define FOCUS_PIXAL_5MM  (917)
//#define FOCUS_PIXAL      FOCUS_PIXAL_5MM

#define USER_CAN_PORT   "can0"
#define USER_CAN_ID      0x11
#define MCU_CAN_ID       0x1F

//根据曝光强度设置曝光时间以适应不同的环境
//未完成，本想使用遗传算法。
#define EXPO_CLASS_5      13000
#define EXPO_CLASS_4      0x11
#define EXPO_CLASS_3      0x11
#define EXPO_CLASS_2      0x11
#define EXPO_CLASS_1      0x11

#endif /* _CONSTANTS_H */
