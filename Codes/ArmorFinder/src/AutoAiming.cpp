#include "AutoAiming.h"

using namespace cv;
using namespace std;

/**
 * @name AutoAiming::run
 * @author seafood
 * @par cv::Mat &g_srcImage,cv::Mat &g_processImage
 * @func    装甲板自运行函数
 * @return void
 * */
void AutoAiming::run(cv::Mat g_srcImage,cv::Mat g_processImage)
{
    //选择状态
    switch(state)
    {
        case SEARCHING_STATE: 
            //搜索状态程序
            searchingProm(g_srcImage,g_processImage);
            break;
        case CLASSIFYING_STATE:
            //分类状态程序 
            classifyingProm(g_srcImage,g_processImage);
            break;
        case TRACKING_STATE:
            //跟踪状态程序  
            trackingProm(g_srcImage,g_processImage);
            break;
    }
}
/**
 * @name        AutoAiming::searching_prom
 * @author      seafood
 * @par         
 * @return     void
 * @function    搜索程序
 * */
void AutoAiming::searchingProm(cv::Mat g_srcImage,cv::Mat g_processImage)
{
    //查找装甲板
    if(statesearchingtarget.run(g_srcImage,g_processImage))
    {
        //模式1
        if(true)
        {
            state = CLASSIFYING_STATE;
        }
        //模式2
        else
        {
            state = TRACKING_STATE;
        }    
    }
}
/**
 * @name        AutoAiming::searching_prom
 * @author      seafood
 * @par         
 * @return     void
 * @function    搜索程序
 * */
void AutoAiming::classifyingProm(cv::Mat g_srcImage,cv::Mat g_processImage)
{
    target_box=stateclassifyingtarget.run(g_srcImage,g_processImage,armor_boxes);
    //查找装甲板
    if(target_box.id!=-1)
    {
        //jump_state=1;jump_state_count=0;
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
    if(mcu_data.env_light > 0 && mcu_data.env_light < 20){
        fExposureTime=1000;
    }
    else if(mcu_data.env_light >= 20 && mcu_data.env_light < 40){
        fExposureTime=3000;
    }
    else if(mcu_data.env_light >= 40 && mcu_data.env_light < 60){
        fExposureTime=5000;
    }
    else if(mcu_data.env_light >= 60 && mcu_data.env_light < 80){
        fExposureTime=7000;
    }
    else{
        fExposureTime=EXPO_CLASS_5;
    }
    nRet = MV_CC_SetFloatValue(handle, "ExposureTime", fExposureTime);
    /*if (MV_OK == nRet)
    {
        printf("set exposure 30 time OK!\n\n");
    }
    else
    {
        printf("set exposure 30 time failed! nRet [%x]\n\n", nRet);
        }*/
}
/**
 * @name        AutoAiming::trackingProm
 * @author      seafood
 * @par         
 * @return     void
 * @function    跟踪状态
 * */
void AutoAiming::trackingProm(cv::Mat g_srcImage,cv::Mat g_processImage)
{
    //查找装甲板
    if(show_state){
        cout<<"追踪启动成功"<<tracking_cnt<<endl;   
    }
    if (!statetrackingtarget.run(g_srcImage,g_processImage,target_box, kalman_filter,tracker,tracking_cnt) || ++tracking_cnt > 100) {    // 最多追踪100帧图像
        //删除卡尔曼滤波
        delete kalman_filter;
        //状态更改为搜寻状态
        state = SEARCHING_STATE;
                
    }
    else{
        tracking_cnt++;
    }
}