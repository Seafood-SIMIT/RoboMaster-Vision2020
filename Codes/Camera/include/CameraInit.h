#ifndef _CAMERA_INIT_H_
#define _CAMERA_INIT_H_

//------------------------包含文件和命名空间-------------------  
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include "MvCameraControl.h"

#include "canManifold2G.h"
#include "constants.h"
#define MAX_IMAGE_DATA_SIZE   (1920*1200*3)

//--------------------------函数定义--------------------
int cameraInit();
int cameraExit();
//------------------------全局变量-----------------------
extern int nRet  ;
extern void* handle ;
//一帧数据大小
extern int nBuffSize;
extern unsigned char* pFrameBuf;            //相机位置
extern float fExposureTime;
#endif