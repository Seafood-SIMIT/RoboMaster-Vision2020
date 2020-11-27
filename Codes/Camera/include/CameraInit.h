#ifndef _CAMERA_INIT_H_
#define _CAMERA_INIT_H_

//------------------------包含文件和命名空间------------------- 
//项目头文件
#include "mcudata.h"
#include "constants.h"      //常量配置  
//平台头文件
#include "MvCameraControl.h"
//标准库
#include <stdio.h>


#define MAX_IMAGE_DATA_SIZE   (1920*1200*3)

//--------------------------函数定义--------------------
int cameraInit();
int cameraExit();
//------------------------变量-----------------------
int nRet;                           //相机运行成功参数
int nBuffSize;                      //一帧数据大小
float fExposureTime;                //曝光时间
void* handle ;
unsigned char* pFrameBuf;            //相机位置
MV_FRAME_OUT_INFO_EX stInfo;        //相机参数

#endif