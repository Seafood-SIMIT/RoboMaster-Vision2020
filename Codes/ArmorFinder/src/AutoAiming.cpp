#include "ArmorFinder.h"

using namespace cv;
using namespace std;


void AutoAiming::run(cv::Mat &g_srcImage,cv::Mat &g_processImage)
{
    switch(state)
    {
        case SEARCHING_STATE: 
            //cout<<"SEARCHING State start!"<<endl;
            if( stateSearchingTarget(g_srcImage,g_processImage) )
            {
                //调高曝光
                if(true)        //这地方到时候加入分类器
                {
                    tracker = TrackerKCF::create();
                    tracker->init(g_srcImage, target_box.rect);
                    //状态改为追踪状态
                    state = TRACKING_STATE;
                    //追踪帧数
                    tracking_cnt = 0;
                }
            }
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