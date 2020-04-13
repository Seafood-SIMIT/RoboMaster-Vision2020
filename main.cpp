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
VideoCapture g_capture("material/video/rmvideodark.MOV");
Preprocess g_preprocess;    //初始化对象
//自瞄程序对象

//数字识别样本特征

int main(int argc, char *argv[], char **env)
{    	
	systemInit();
    int width_fig, height_fig;      //图像的宽和高
    Mat g_srcImage, g_processImage, g_highexposure;     //原图像和预处理图像
    AutoAiming auto_aiming;             //自瞄对象
    auto_aiming.state = AutoAiming::State::SEARCHING_STATE;     //自瞄吃状态

    while(1)
    {
        switch(g_source_type)
        {
        case SOURCE_CAMERA:
        {

            nRet = MV_CC_GetImageForBGR(handle, pFrameBuf, nBuffSize, &stInfo, 1000);
            if( nRet != 0 )
            {
                cout << "error:GetImageForRGB:" << setbase(16) << nRet << endl;
			    break;
            }
            else
            {
			    width_fig = stInfo.nWidth;
			    height_fig = stInfo.nHeight;
 
			    if (stInfo.enPixelType == PixelType_Gvsp_BGR8_Packed)
			    {
				    Mat pImg(height_fig, width_fig, CV_8UC3, pFrameBuf);
                    g_srcImage = pImg;
                    
			    }
                
            }
            break;
        }
        case SOURCE_VIDEO:
        {
            g_capture.read(g_srcImage);
            break;
        }
        }
        //
        if(auto_aiming.jump_state == 1 && auto_aiming.jump_state_count <3)
        {
            auto_aiming.jump_state_count++;
            continue;
        }
        namedWindow("g_srcImage",0);
        resizeWindow("g_srcImage",600,400);
        //cout<<"one turn"<<endl;
        g_processImage = g_srcImage.clone();
        
        g_preprocess.run(g_processImage);
        //cout<<"one turn"<<endl;
        auto_aiming.run(g_srcImage,g_processImage);
        //cout<<"one turn"<<endl;
        //cvtColor(g_srcImage,g_srcImage,COLOR_RGB2GRAY);
        imshow("g_srcImage",g_srcImage);
        waitKey(1);
        //energy(g_srcImage);
        
    }
    cameraExit();
    return 0;
}

void systemInit()
{
    //输入源选择
    cout << "Input 1 for camera, 0 for video" << endl;
    cin >> g_source_type;
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
        //g_capture.open();//视频文件
        //图像读取
        //g_capture.read("material/video/rmvideodark.MOV");
    }
    //数字样本集采集
    int count_number=0, filename=0;
    
    for(int i = 1; i < 9; i++)//在原例子上，i=0;i<10改成i=1;i<9
    {
        for(int j=1;j<4;j++)
        {
            filename=i*10+j;
            string s = "material/picture/number/" + to_string(filename) + ".png";//to_string(k)：将数值k转化为字符串，返回对应的字符串
            //string s ="material/picture/number/13.png";
            Mat num_yangben = imread(s,1);
            //cout<<"read file "<<filename<<" type is "<<num_yangben.type()<<endl;
            getFeature(num_yangben, yangben_Feature[count_number]);
            //cout<<"read file "<<filename<<" Feature is "<<yangben_Feature[i][10]<<endl;
            count_number++;
        }
    }
}
