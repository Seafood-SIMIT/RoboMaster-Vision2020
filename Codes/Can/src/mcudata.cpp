//
// Created by sjturm on 19-5-17.
//

#include "canManifold2G.h"
using namespace std;
/**
 * @name    :canReceive
 * @author  :seafood
 * @func    :can总线接收线程
 * @parm    :CAN类型对象
 * @return  :void
 * *****************/
void canReceive(Can *pCan) 
{
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