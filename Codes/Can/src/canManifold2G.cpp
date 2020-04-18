/*
 * cantest.c
 *
 *  Created on: 2020/4/14
 *      Author: seafood
 * 		CAN总线源文件
 */
#include "canManifold2G.h"

int enemy_color = ENEMY_RED;
/**
 * @name CANSend
 * @author seafood
 * @par		data[]
 * @func	CAN总线发送函数
 * */
int CANSend(unsigned char data[])
{
	int s, nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	//struct can_frame frame0;
	struct can_frame frame1;
	//struct can_filter rfilter[1];

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)//创建套接字
	{
	    perror("Create socket failed");
	    exit(-1);
	}
	
	strcpy(ifr.ifr_name, "can0"); //设置can接口
	printf("can port is %s\n",ifr.ifr_name);
	ioctl(s, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
    
	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) //can接口和套接字关联
	{
		perror("Bind can device failed\n");
		close(s);
		exit(-2);
	}
   	frame1.can_id = 0x1F;  //发送数据设置
    frame1.can_dlc = 8;
    printf("%s ID=%#x data length=%d\n", ifr.ifr_name, frame1.can_id, frame1.can_dlc);
    /* prepare data for sending: 0x11,0x22...0x88 */
    	for (int i=0; i<8; i++)
         {
        	frame1.data[i] = data[i];
        	printf("%#x ", frame1.data[i]);
         }

        printf("Sent out\n");
        /* Sending data */
        if(write(s, &frame1, sizeof(frame1)) < 0) //发送
		{
            perror("Send failed");
            close(s);
		}
	 close(s);
}
// thread receive(canReceive);//开启串口接收线程
/**
 * @name CANRecv
 * @author seafood
 * @par		
 * @func	CAN总线接收函数
 * */
void CANRecv()
{
	int s, nbytes;
	char buffer[8];
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame0;
	//struct can_frame frame1;
	struct can_filter rfilter[1];

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)//创建套接字
	{
	    perror("Create socket failed");
	    exit(-1);
	}
	
	strcpy(ifr.ifr_name, "can0"); //设置can接口
	printf("can port is %s\n",ifr.ifr_name);
	ioctl(s, SIOCGIFINDEX, &ifr);//绑定can0设备
    addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
    
	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) //can接口和套接字关联
	{
		perror("Bind can device failed\n");
		close(s);
		exit(-2);
	}
    rfilter[0].can_id = 0x1F; //设置滤波器，仅接收can id 0x1F的数据
	rfilter[0].can_mask = CAN_SFF_MASK;
	if(setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)) < 0)
	    {
	    	perror("set receiving filter error\n");
	    	close(s);
	    	exit(-3);
	    }

	    while(1){
	        nbytes = read(s, &frame0, sizeof(frame0)); //接收
			if((frame0.can_id==0x1F)&&(nbytes>0))
			{
		    	printf("%s ID=%#x data length=%d\n", ifr.ifr_name, frame0.can_id, frame0.can_dlc);
	        	for (int i=0; i < frame0.can_dlc; i++)
	        		printf("%#x ", frame0.data[i]);
	        	printf("\n");
				memcpy(buffer,frame0.data,nbytes);
				//
                enemy_color=int(buffer[5]);
	        }
	    }
}
