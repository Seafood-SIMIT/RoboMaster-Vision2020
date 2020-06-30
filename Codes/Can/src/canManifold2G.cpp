/*
 * cantest.c
 *
 *  Created on: 2020/4/14
 *      Author: seafood
 * 		CAN总线源文件
 */
#include "canManifold2G.h"

/**
 * @name CANSend
 * @par		data[]
 * @func	CAN总线发送函数
 * */
int Can::canTansfer(unsigned char data[])
{
	int s1;
	struct sockaddr_can addr1;
	struct ifreq ifr1;
	struct can_frame frame1;
	if ((s1 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)//创建套接字
	{
	    perror("Create socket failed");
	    exit(-1);
	}
	
	strcpy(ifr1.ifr_name, "can0"); //设置can接口
	//printf("can port is %s\n",ifr1.ifr_name);
	ioctl(s1, SIOCGIFINDEX, &ifr1);
    addr1.can_family = AF_CAN;
	addr1.can_ifindex = ifr1.ifr_ifindex;
    
	if(bind(s1, (struct sockaddr *)&addr1, sizeof(addr1)) < 0) //can接口和套接字关联
	{
		perror("Bind can device failed\n");
		close(s1);
		exit(-2);
	}
   	frame1.can_id = 0x1F;  //发送数据设置
    frame1.can_dlc = 8;
    //printf("%s ID=%#x data length=%d\n", ifr1.ifr_name, frame1.can_id, frame1.can_dlc);
    /* prepare data for sending: 0x11,0x22...0x88 */
    	for (int i=0; i<8; i++)
         {
        	frame1.data[i] = data[i];
        	//printf("%#x ", frame1.data[i]);
         }

        //printf("Sent out\n");
        /* Sending data */
        if(write(s1, &frame1, sizeof(frame1)) < 0) //发送
		{
            perror("Send failed");
            close(s1);
		}
	 close(s1);
}
// thread receive(canReceive);//开启串口接收线程
/**
 * @name CANRecv
 * @par		
 * @func	CAN总线接收函数
 * */
int Can::canReadData(unsigned char *buffer) {
	int s0, nbytes;
	struct sockaddr_can addr0;
	struct ifreq ifr0;
	struct can_frame frame0;
	struct can_filter rfilter[1];
	if ((s0 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)//创建套接字
	{
	    perror("Create socket failed");
	    exit(-1);
	}
	
	strcpy(ifr0.ifr_name, "can0"); //设置can接口
	//printf("can port is %s\n",ifr0.ifr_name);
	ioctl(s0, SIOCGIFINDEX, &ifr0);//绑定can0设备
    addr0.can_family = AF_CAN;
	addr0.can_ifindex = ifr0.ifr_ifindex;
    
	if(bind(s0, (struct sockaddr *)&addr0, sizeof(addr0)) < 0) //can接口和套接字关联
	{
		perror("Bind can device failed\n");
		close(s0);
		exit(-2);
	}
    rfilter[0].can_id = 0x1F; //设置滤波器，仅接收can id 0x1F的数据
	rfilter[0].can_mask = CAN_SFF_MASK;
	if(setsockopt(s0, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)) < 0)
	    {
	    	perror("set receiving filter error\n");
	    	close(s0);
	    	exit(-3);
	    }
    nbytes = read(s0, &frame0, sizeof(frame0)); //接收
	if((frame0.can_id==0x1F)&&(nbytes>0))
		{
	    	//printf("%s ID=%#x data length=%d\n", ifr0.ifr_name, frame0.can_id, frame0.can_dlc);
	        	//for (int i=0; i < frame0.can_dlc; i++)
	        		//printf("%#x ", frame0.data[i]);
	        	//printf("\n");
				memcpy(buffer,frame0.data,nbytes);
	        }
	else{
		nbytes=0;
		}
		return nbytes;
	    
}
/**
 * @name        sendPosition
 * @author      seafood
 * @par         cv::Rect2d &rect,Mat src
 * @return      void信息
 * @function    发送装甲板 zifuchuan 
 * */
void sendPosition(cv::Rect2d rect_target,cv::Mat srcImage){
	//y要发送的字符串
    unsigned char data[8];
    // double relative_x=box.getCenter().x-640;
    // double relative_y=box.getCenter().y-512;
    //cout<<"到这"<<endl;
    data[0]=(int(rect_target.x + rect_target.width / 2)>>8)&0xFF;
    data[1]=(int(rect_target.x + rect_target.width / 2))&0xFF;
    data[2]=(int(rect_target.y + rect_target.height / 2)>>8)&0xFF;
    data[3]=(int(rect_target.y + rect_target.height / 2))&0xFF;
    data[4]=((int((srcImage.cols)/2))>>8)&0xFF;
    data[5]=(int((srcImage.cols)/2))&0xFF;
    data[6]=((int((srcImage.rows)/2))>>8)&0xFF;
    data[7]=(int((srcImage.rows)/2))&0xFF;
    can.canTansfer(data);
    //cout<<"Send Messege"<<endl;
    // cout<<"x"<<relative_x<<",y:"<<relative_y<<endl;
}