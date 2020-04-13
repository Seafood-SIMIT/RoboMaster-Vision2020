/*-----------------------------文件--------------------
*   文件名：Preprocess.cpp
*   作者：  孙霖
*   功能：  载入图像预处理，目前实现对日光滤出
------------------------------------------------------*/

#include "Preprocess.h"

void Preprocess::run(Mat &g_processImage)
{
    clearWhiteLight(g_processImage);                                //消除白光
    cvtColor(g_processImage, g_processImage, COLOR_GRAY2RGB);     //将黑白图像转换成三通道
    /*
    namedWindow("g_processImage",0);
    resizeWindow("g_processImage",600,400);
    imshow("g_processImage",g_processImage);
    waitKey(1);*/
}

void Preprocess::clearWhiteLight(Mat &g_processImage)
{
    Mat g_processImage_blue,g_processImage_red;       //红蓝通道图像
    if (g_processImage.type() == CV_8UC3)
    {
        vector<Mat> channels;                 /*利用vector对象拆分*/
        split(g_processImage, channels);                     /*调用通道拆分函数*/
        g_processImage_red = channels[2];             /*将红色提出来，红色是第三个通道*/   
        g_processImage_blue = channels[0];            /*将蓝色提出来，红色是第一个通道*/ 
        addWeighted(g_processImage_red, 1, g_processImage_blue,-1, 0.0, g_processImage);     //将两张图片按比例合成一张图片
    } 
    
    
}

//大津发
//计算大津法的参数thresh
uchar Otsu(Mat &img)
{
	int i,j;
	//行列
	int row = img.rows;
	int col = img.cols;
	double rc = row*col;
	//存储各级灰度的个数 初始为0
	int n[256] = {0};
	uchar *ptr;
	for(i = 0; i < row; ++i)  
    {  
        ptr = img.ptr<uchar>(i);  
        for (j = 0; j < col; ++j)  
        {  
            n[ptr[j]]++;
        }  
    }  
	
	//归一化 并且 计算累积和 、 累计均值 和 全局均值 
	double p[256];   
	double m[256];
	double mg;
 
	//计算概率的时候注意 如果都是整型 结果为0 
	p[0] = n[0]/rc;
	m[0] = 0;
	mg = 0;
 
    for(i = 1;i < 256; i++)
	{
		//计算概率 求全局均值
		p[i] = n[i]/rc;
		m[i] = i*p[i];
		mg += m[i];
        //进行累加
        p[i]+=p[i-1];
		m[i]+=m[i-1];
	}
 
	//阈值
	uchar k = 0;
	//类间方差 
	double a = 0,a2;
	for(i = 0;i < 256; i++)
	{       //这里少加个判断 if(p[i]==0||p[i]==1) continue;
		a2 = (mg*p[i]-m[i])*(mg*p[i]-m[i])/(p[i]*(1-p[i]));    
		if(a2 > a)
		{
			k = i;
			a = a2;
		}
	}
	return k;
}