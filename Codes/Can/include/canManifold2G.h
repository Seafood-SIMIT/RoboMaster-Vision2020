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

#include "constants.h"
#include "mcudata.h"
int CANSend(unsigned char data[]);      //CAN发送函数
void CANRecv();                          //CAN接收函数

//mcu传输数据初始化结构体
extern McuData mcu_data;

#endif