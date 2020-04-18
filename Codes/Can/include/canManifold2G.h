#ifndef CAN
#define CAN

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
extern int enemy_color;

int CANSend(unsigned char data[]);      //CAN发送函数
void CANRecv();                          //CAN接收函数

#endif