#ifndef USER_CAN_H
#define USER_CAN_H
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fstream>
#include <cstring>
#include "constants.h"
*/
#include <opencv2/opencv.hpp>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <unistd.h>
//Canjiegouti
class Can {
public:
    Can(){};
    ~Can(){};
    int canTansfer(unsigned char data[]);
    int canReadData(unsigned char* buffer);
};
static Can can;

//发送坐标点数据结构
void sendPosition(cv::Rect2d rect,cv::Mat src);

#endif