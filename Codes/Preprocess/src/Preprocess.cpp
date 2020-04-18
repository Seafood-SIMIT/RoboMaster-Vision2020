/*-----------------------------文件--------------------
*   文件名：Preprocess.cpp
*   作者：  孙霖
*   功能：  载入图像预处理，目前实现对日光滤出
------------------------------------------------------*/

#include "Preprocess.h"
/**
 * @name        Preprocess::run
 * @author      seafood
 * @par         Mat &g_processImage
 * @return      void
 * @function    对输入光源进行预处理,去白光.
 * */
void Preprocess::run(Mat &g_processImage)
{
    clearWhiteLight(g_processImage);                                //消除白光
    cvtColor(g_processImage, g_processImage, COLOR_GRAY2RGB);     //将黑白图像转换成三通道
    if(show_preprocess)
	{
		namedWindow("g_processImage",0);
    	resizeWindow("g_processImage",600,400);
    	imshow("g_processImage",g_processImage);
    	waitKey(1);
	}
}
/**
 * @name        Preprocess::clearWhiteLight
 * @author      seafood
 * @par         Mat &g_processImage
 * @return      void
 * @function    对输入光源进行预处理,去白光.
 * */
void Preprocess::clearWhiteLight(Mat &g_processImage)
{
    Mat g_processImage_blue,g_processImage_red;       //红蓝通道图像
    vector<Mat> channels;                 /*利用vector对象拆分*/
    split(g_processImage, channels);                     /*调用通道拆分函数*/
    g_processImage_red = channels[2];             /*将红色提出来，红色是第三个通道*/   
    g_processImage_blue = channels[0];            /*将蓝色提出来，红色是第一个通道*/ 
	//根据敌方颜色消除通道
    if(mcu_data.enemy_color = ENEMY_RED)
	{
		addWeighted(g_processImage_red, 1, g_processImage_blue,-1, 0.0, g_processImage);     //将两张图片按比例合成一张图片
	}
    else if(mcu_data.enemy_color = ENEMY_BLUE)
	{
		addWeighted(g_processImage_red, 1, g_processImage_blue,-1, 0.0, g_processImage);     //将两张图片按比例合成一张图片
	}
}