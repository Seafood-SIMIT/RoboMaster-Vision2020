#include "ArmorFinder.h"

using namespace cv;
using namespace std;

/**
 * @name AutoAiming::run
 * @author seafood
 * @par cv::Mat &g_srcImage,cv::Mat &g_processImage
 * @func    装甲板自运行函数
 * @return void
 * */
void AutoAiming::run(cv::Mat &g_srcImage,cv::Mat &g_processImage)
{
    //选择状态
    switch(state)
    {
        //搜索状态
        case SEARCHING_STATE: 
            //搜索状态
            cout<<"SEARCHING State start!"<<endl;
            if( stateSearchingTarget(g_srcImage,g_processImage) )       //搜索到装甲板
            {
                //调高曝光
                fExposureTime = 70000;
                nRet = MV_CC_SetFloatValue(handle, "ExposureTime", fExposureTime);
                //sleep(1);//延时1秒
                //waitKey(1000);
                /*if (MV_OK == nRet)
                {
                    printf("set exposure 70000 time OK!\n\n");
                }
                else
                {
                    printf("set exposure 70000 time failed! nRet [%x]\n\n", nRet);
                }*/
                //提高曝光
                //状态改为分类状态
                state = CLASSIFYING_STATE;
                cout<<"Changed into CLASSIFYING STATE"<<endl;
                
            }
            break;
        case CLASSIFYING_STATE:
            //分类状态
            cout<<"CLASSIFYING State start!"<<endl;
            //识别ROI中的数字
            if(numberClassifyRoi(g_srcImage, g_processImage))
            {
                jump_state=1;jump_state_count=0;
                //sleep(1);//延时1秒
                //waitKey(1000);
                //初始化追踪器
                tracker = TrackerKCF::create();
                //tracker = TrackerMIL::create();
                tracker->init(g_srcImage, target_box.rect);
                state = TRACKING_STATE;
                //追踪帧数
                tracking_cnt = 0;
            }
            else
            {
                state = SEARCHING_STATE;
            }
            fExposureTime=1000;
            nRet = MV_CC_SetFloatValue(handle, "ExposureTime", fExposureTime);
            /*if (MV_OK == nRet)
            {
                printf("set exposure 30 time OK!\n\n");
            }
            else
            {
                printf("set exposure 30 time failed! nRet [%x]\n\n", nRet);
            }*/
            break;
        case TRACKING_STATE:
            //cout<<"Tracking State start!"<<endl;   
            cout<<"追踪启动成功"<<tracking_cnt<<endl;   
            if (!stateTrackingTarget(g_srcImage,g_processImage) || ++tracking_cnt > 100) {    // 最多追踪100帧图像
                state = SEARCHING_STATE;
            }
            break;
    }
}