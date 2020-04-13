#ifndef MAIN_H_
#define MAIN_H_
//------------------------包含文件-------------------  
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "MvCameraControl.h"    //相机函数

//包含自制头文件
#include "CameraInit.h"         //相机初始化函数
#include "Preprocess.h"         //图像预处理
#include "energy.h"             //能量机关
#include "ArmorFinder.h"        //搜寻装甲板
#include "GetFeature.h"

#define MAX_IMAGE_DATA_SIZE   (1920*1200*3)     //图像最大大小
#define SOURCE_CAMERA    1      //信号源为摄像机
#define SOURCE_VIDEO    0       //信号源为视频源
#define CAMERA_INIT_SUCCESS 1   //相机初始化成功
#define CAMERA_INIT_FAIL    0   //相机初始化失败
//------------------------全局变量定义---------------------
//变量定义阶段
int g_source_type;              //源选择函数

void systemInit();              //系统初始化函数


#endif