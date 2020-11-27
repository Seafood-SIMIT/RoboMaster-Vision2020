/*-----------------------------文件--------------------
*   文件名：main.cpp
*   作者：  seafood
*   功能：  主函数
------------------------------------------------------*/
//------------------------包含文件-------------------
//项目头文件

#include "Preprocess.h"         //图像预处理
#include "energy.h"             //能量机关

#include "log.h"                //log
#include "AutoAiming.h"

#include <thread>
#include <istream>

//-----------------------命名空间------------------------
using namespace cv;
using namespace std;
//---------------------------------------------------//


/**
 * @name        void systemInit
 * @author      seafood
 * @par         none
 * @return      null
 * @function    系统初始化,包括源的选择和输入数字特征
 * */
cv::VideoCapture systemInit(int* source_type)
{
    

    //g_source_type = SOURCE_CAMERA;
    //输入源选择
    //默认摄像机哦
    if(choose_source_button)
    {
        //运行交互语句
        cout << "Input 1 for camera, 0 for video" << endl;
        cin >> *source_type;
    }
    cv::VideoCapture capture;         //视频源声明
        //源文件地址
    if(*source_type == SOURCE_CAMERA)
    {

        //初始化摄像机
        if( cameraInit() == CAMERA_INIT_SUCCESS)
        {
            //cout << "camera init success" << endl;
            //获取一帧数据的大小
            MVCC_INTVALUE stIntvalue = {0};
            nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stIntvalue);

            if (nRet != MV_OK)
            {
                printf("Get PayloadSize failed! nRet [%x]\n", nRet);
            }	
	        //申请空间
            pFrameBuf = (unsigned char*)malloc(nBuffSize);

            //设置存储区
            memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
        }
        //return NULL;
    }
    else
    {
        capture.open("material/video/rmvideo.MOV");
        
    }
    //数字样本集采集
    getYangbenFeatures();
    return capture;
}
/**
 * @name        void systemInit
 * @par         none
 * @return      int
 * @function    can接收线程
 * */
int CanThreadReceieve(){
//声明一个can对象
    //Can can;
    //握手信息声明.
    unsigned char handshake[]= "2020";
    //发送handshake包
    for(int i = 0; i < 3; i++){
        int res = can.canTansfer(handshake);        //can总线发送三次handshake等待电控组回应
        waitKey(500);                               //延时500ms
    }
    cout<<"Send Can Mes Sucess."<<endl;
    thread receive(mcuDataReceive,&can);            //开启线程接收数据
    receive.detach();
    return 1;
}
/**
 * @name        int main
 * @author      seafood
 * @par         int argc, char *argv[], char **env
 * @return      0
 * @function    系统运行主程序
 * */
int main(int argc, char *argv[], char **env)
{   
    processOptions(argc, argv);                     // 处理命令行参数
    //变量定义
    int source_type;              //源选择函数
    cv::VideoCapture capture;         //视频源声明
    //初始化预处理函数对象
    Preprocess g_preprocess;                        //初始化对象
    int width_fig, height_fig;                      //图像的宽和高
    cv::Mat g_srcImage, g_processImage, g_highexposure; //原图像和预处理图像
    AutoAiming auto_aiming(g_srcImage,g_processImage);                         //自瞄对象
    auto_aiming.state = State::SEARCHING_STATE;     //自瞄初状态

    capture = systemInit(&source_type);                                   //系统初始化
    if(run_with_can){
        CanThreadReceieve();
    }

    //主程序循环
    while(1)
    {
        //选择信号源
        switch(source_type)
        {
        //源为相机
        case SOURCE_CAMERA:
        {
            //读取RGB图像
            nRet = MV_CC_GetImageForBGR(handle, pFrameBuf, nBuffSize, &stInfo, 1000);
            if( nRet != 0 )
            {
                //失败error输出
                cout << "error:GetImageForRGB:" << setbase(16) << nRet << endl;
			    break;
            }
            //读取图像成功
            else
            {
                //图像大小
                //每次图像大小一样放在这里确实不妥
			    width_fig = stInfo.nWidth;
			    height_fig = stInfo.nHeight;
                //从存储中读取图像
			    if (stInfo.enPixelType == PixelType_Gvsp_BGR8_Packed)
			    {
				    Mat pImg(height_fig, width_fig, CV_8UC3, pFrameBuf);
                    g_srcImage = pImg;                          //src图像储存
                    g_processImage = g_srcImage.clone();        //预处理图像克隆(看效果决定放置位置)
                    
			    }
                
            }
            break;
        }
        //源为视频
        case SOURCE_VIDEO:
        {
            //读取视频
            capture.read(g_srcImage);
            break;
        }
        }
        //追踪模式跳过前三帧
        //后期优化时删除或者更改
        //if(auto_aiming.jump_state == 1 && auto_aiming.jump_state_count <3)
        //{
        //    auto_aiming.jump_state_count++;
        //    continue;
        //}
        //若参数为显示src图像
        
        if(show_origin)
        {
            namedWindow("g_srcImage",0);
            resizeWindow("g_srcImage",600,400);
            imshow("g_srcImage",g_srcImage);
            waitKey(1);
        }
        //图像预处理
        g_preprocess.run(g_processImage);
        //自瞄程序
        // cout<<g_srcImage.rows<<","<<g_srcImage.cols<<endl;
        // sleep(30);
        auto_aiming.run(g_srcImage,g_processImage);
        
    }
    //退出相机
    cameraExit();
    //退出
    return 0;
}

