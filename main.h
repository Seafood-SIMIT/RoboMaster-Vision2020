#ifndef MAIN_H_
#define MAIN_H_
//------------------------包含文件和命名空间-------------------  
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MvCameraControl.h"

//包含自制头文件
#include "CameraInit.h"
#include "Preprocess.h"
#include "energy.h"

using namespace cv;
using namespace std;


#define MAX_IMAGE_DATA_SIZE   (1920*1200*3)
#define SOURCE_CAMERA    1
#define SOURCE_VIDEO    0
#define CAMERA_INIT_SUCCESS 1
#define CAMERA_INIT_FAIL    0
int lightBox(Mat image);
void systemInit();
//------------------------全局变量定义---------------------
//变量定义阶段
int g_source_type;
VideoCapture g_capture;
Preprocess g_preprocess;    //初始化对象

//extern int nRet;    //相机状态定义
//extern void* handle;    //
//一帧数据大小
int nBuffSize = MAX_IMAGE_DATA_SIZE;
unsigned int nTestFrameSize = 0;
//相机位置
unsigned char* pFrameBuf = NULL;

//相机参数
MV_FRAME_OUT_INFO_EX stInfo;

#endif