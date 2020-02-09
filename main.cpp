/*-----------------------------文件--------------------
*   文件名：main.cpp
*   作者：  孙霖
*   功能：  主函数
------------------------------------------------------*/
#include "main.h"
//-----------------------主函数定义------------------------
using namespace cv;
using namespace std;

//自瞄程序对象

int main(int argc, char *argv[], char **env)
{    	
	systemInit();
    Mat g_srcImage, g_processImage;
    AutoAiming auto_aiming;
    auto_aiming.state = AutoAiming::State::SEARCHING_STATE;
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
			    int width = stInfo.nWidth;
			    int height = stInfo.nHeight;
 
			    if (stInfo.enPixelType == PixelType_Gvsp_BGR8_Packed)
			    {
				    Mat pImg(height, width, CV_8UC3, pFrameBuf);
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
        
        namedWindow("g_srcImage",0);
        resizeWindow("g_srcImage",600,400);
        g_processImage = g_srcImage.clone();
        g_preprocess.run(g_processImage); 
        auto_aiming.run(g_srcImage,g_processImage);
        imshow("g_srcImage",g_srcImage);
        waitKey(1);
        //energy(g_srcImage);
        
        
        
        
    }
    cameraExit();
    return 0;
}

void systemInit()
{
    cout << "Input 1 for camera, 0 for video" << endl;
    cin >> g_source_type;
    if(g_source_type == 1)
    {
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
        //g_capture.open("material/video/rmvideo.MOV");//视频文件
        //视频读取
        //g_capture.read()
    }
}
