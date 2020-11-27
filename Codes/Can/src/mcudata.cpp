/*-----------------------------文件--------------------
*   文件名：    mcudata.cpp
*   作者：      seafood
*   功能：      与mcu之间交互函数
------------------------------------------------------*/
#include "mcudata.h"
using namespace std;

/**
 * @name    :canReceive
 * @author  :seafood
 * @func    :can总线接收线程
 * @parm    :CAN类型对象
 * @return  :void
 * *****************/
void mcuDataReceive(Can *pCan) 
{
    //mcu_data初始化
    mcu_data={          // 单片机端回传结构体初始化
            0,              // 当前大云台yaw角度
            0,              // 当前大云台pitch角度
            0,              // 当前小云台yaw角度
            0,              // 当前小云台pitch角度
            0,              // 子弹速度
            0,              // 光照强度
            0,              // 大/小云台工作模式 0x00：大云台直射，小云台全自动  0x01:大云台抛射，小云台全自动
            0,              // 前4位表示工作状态，后4位表示敌方战车颜色
            };

    char buffer[8];
    int k=0; //计时变量
    while (true) {
        //cout<<"waiting MCU"<<endl;
        if(k==50)
        {
            int j;
            k=0;
            unsigned char resend[]="handshake"; //k=50,说明1秒内没有从单片机接收到正确的数据
            for(j=0;j<3;j++)                   //则重新发送初始信号，再次建立通信
            pCan->canTansfer(resend);
        }
        memset(buffer, 0, sizeof(buffer));  //初始化buffer数组

        int nbytes = pCan->canReadData((uint8_t *) buffer);
        
        if (nbytes==8)  //接收到8个字节的数据，通信正确
        {
            //cout<<"receieve 8 data"<<endl;
            k=0;
            memcpy(&mcu_data, buffer, sizeof(mcu_data)); //接收到的数据复制到结构体中
        }
        else
        {
            k++;
        }
        usleep(20000);//延时等待20毫秒
    }
}