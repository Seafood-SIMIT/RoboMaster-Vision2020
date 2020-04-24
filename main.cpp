/*-----------------------------文件--------------------
*   文件名：main.cpp
*   作者：  孙霖
*   功能：  主函数
------------------------------------------------------*/
#include "main.h"
//-----------------------命名空间------------------------
using namespace cv;
using namespace std;
//---------------------------------------------------//
//相机参数
MV_FRAME_OUT_INFO_EX stInfo;

/**
 * @name        int main
 * @author      seafood
 * @par         int argc, char *argv[], char **env
 * @return      0
 * @function    系统自瞄运行主程序
 * */
int main(int argc, char *argv[], char **env)
{    
    processOptions(argc, argv);             // 处理命令行参数
    //变量定义
    //初始化预处理函数对象
    Preprocess g_preprocess;    //初始化对象
    int width_fig, height_fig;      //图像的宽和高
    Mat g_srcImage, g_processImage, g_highexposure;     //原图像和预处理图像
    AutoAiming auto_aiming;             //自瞄对象
    auto_aiming.state = AutoAiming::State::SEARCHING_STATE;     //自瞄吃状态

    systemInit();                           //系统初始化
    if(run_with_can){
        //发送handshake包
        for(int i = 0; i < 3; i++){
        int res = CANSend(handshake);
        waitKey(500);
        }
    	
        thread receive(CANRecv);                       //开启线程接收数据
        receive.detach();
    }

    //主程序循环
    while(1)
    {
        switch(g_source_type)
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
            //成功
            else
            {
                //图像大小
			    width_fig = stInfo.nWidth;
			    height_fig = stInfo.nHeight;
                //从存储中读取图像
			    if (stInfo.enPixelType == PixelType_Gvsp_BGR8_Packed)
			    {
				    Mat pImg(height_fig, width_fig, CV_8UC3, pFrameBuf);
                    g_srcImage = pImg;                      //src图像储存
                    g_processImage = g_srcImage.clone();        //预处理图像克隆(看效果决定放置位置)
                    
			    }
                
            }
            break;
        }
        //源为视频
        case SOURCE_VIDEO:
        {
            //读取视频
            g_capture.read(g_srcImage);
            break;
        }
        }
        //追踪模式跳过前三帧
        //后期优化时删除或者更改
        if(auto_aiming.jump_state == 1 && auto_aiming.jump_state_count <3)
        {
            auto_aiming.jump_state_count++;
            continue;
        }
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
        auto_aiming.run(g_srcImage,g_processImage);
        
    }
    //退出相机
    cameraExit();
    //退出
    return 0;
}
/**
 * @name        void systemInit
 * @author      seafood
 * @par         none
 * @return      null
 * @function    系统初始化,包括源的选择和输入数字特征
 * */
void systemInit()
{
    g_source_type = SOURCE_CAMERA;
    //输入源选择
    //默认摄像机哦
    if(choose_source_button)
    {
        cout << "Input 1 for camera, 0 for video" << endl;
        cin >> g_source_type;
    }

    if(g_source_type == SOURCE_CAMERA)
    {
        //初始化摄像机
        if( cameraInit() == CAMERA_INIT_SUCCESS)
        {
            cout << "camera init success" << endl;
            //获取一帧数据的大小
            MVCC_INTVALUE stIntvalue = {0};
            nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stIntvalue);

            if (nRet != MV_OK)
            {
                printf("Get PayloadSize failed! nRet [%x]\n", nRet);
            }	
	        
            pFrameBuf = (unsigned char*)malloc(nBuffSize);

            //
            memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
        }
    }
    else
    {
        //源文件地址
        g_capture.open("material/video/rmvideo.MOV");
    }
    //数字样本集采集
    int count_number=0, filename=0;
    
    for(int i = 1; i < 9; i++)//在原例子上，i=0;i<10改成i=1;i<9
    {
        for(int j=1;j<4;j++)
        {
            //文件名
            filename=i*10+j;
            string s = "material/picture/number/" + to_string(filename) + ".png";//to_string(k)：将数值k转化为字符串，返回对应的字符串
            Mat num_yangben = imread(s,1);          //读取文件
            getFeature(num_yangben, yangben_Feature[count_number]);         //获取样本特征
            count_number++;
        }
    }
}
