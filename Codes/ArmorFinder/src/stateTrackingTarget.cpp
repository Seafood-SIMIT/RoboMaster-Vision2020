
/*-----------------------------文件--------------------
*   文件名：LightBox.cpp
*   作者：  王妍璐 江培玲
*   功能：  对图片进行通道拆分和腐蚀膨胀以提取出灯条
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------
#include "stateTrackingTarget.h"
using namespace cv;
using namespace std;
/**
 * @name        AutoAiming::stateTrackingTarget
 * @author      seafood
 * @par         cv::Mat &srcImage,cv::Mat &processImage
 * @return      bool
 * @function    追踪状态主函数
 * */
bool StateTrackingTarget::run(cv::Mat &srcImage,cv::Mat &processImage,ArmorBox &target_box, Kalman *kalman_filter,cv::Ptr<cv::Tracker> tracker,int tracking_cnt) 
{
    //cv的追踪操作
    //位置目标
    auto pos = target_box.rect;
    
    if(!tracker->update(srcImage, pos)){ // 使用KCFTracker进行追踪
        //追踪失败
        //target_box = ArmorBox();
        //LOGW("Track fail!");
        cout<<"Track fail!"<<endl;
        return false;
    }
    //追踪出界
    if((pos & cv::Rect2d(0, 0, srcImage.cols, srcImage.rows)) != pos){
        //target_box = ArmorBox();
        //LOGW("Track out range!");
        cout<<"Track out range!"<<endl;
        return false;
    }
    //cout<<"box的大小位置x:"<<target_box.rect.x<<" y:"<<target_borect.y<<endl;   
    // 我的追踪操作
    // 识别失败使用卡尔曼的值,识别成功发送识别的值.
    // 获取相较于追踪区域两倍长款的区域，用于重新搜索，获取灯条信息
    cv::Rect2d bigger_rect;
    bigger_rect.x = pos.x - pos.width / 2.0;
    bigger_rect.y = pos.y - pos.height / 2.0;
    bigger_rect.height = pos.height * 2;
    bigger_rect.width  = pos.width * 2;
    bigger_rect &= cv::Rect2d(0, 0, 1920, 1080);
    //cout<<"bigger的大小位置x:"<<bigger_rect.x<<" y:"<<bigger_rect.y<<endl;
    //cout<<"picture的大小x:"<<srcImage.rows<<" y:"<<srcImage.cols<<endl;
    cv::Mat roi_src = srcImage(bigger_rect).clone();
    //cout<<"追踪更新成功"<<tracking_cnt<<endl;
    cv::Mat roi_process = processImage(bigger_rect).clone();
    // 在区域内重新搜索。
    //cout<<"box的大小位置x:"<<box.rect.x<<" y:"<<box.rect.y<<endl;
    if(state_searching_target_tracking.run(roi_src,roi_process)) { 
        // 如果成功获取目标，则利用搜索区域重新更新追踪器
        target_box = state_searching_target_tracking.armor_boxes[0];
        //将box的坐标转换为在整幅图片的坐标
        target_box.rect.x += bigger_rect.x; //　添加roi偏移量
        target_box.rect.y += bigger_rect.y;
        //灯条坐标转换
        //此处认为坐标转换只需装甲板坐标即可
        /*
        for(auto &blob : target_box.light_blobs){
            blob.rect.center.x += bigger_rect.x;
            blob.rect.center.y += bigger_rect.y;
        }*/
        //新建卡尔曼追踪模型
        tracker = TrackerKCF::create();
        //追踪初始化
        //这里到时候看一下用process效果好还是src效果好
        tracker->init(srcImage, target_box.rect);
    }
    else{    
        // 如果没有成功搜索目标，则使用卡尔曼预测代替识别值.
        Point pos_track_filed = kalman_filter->predict(Point(target_box.rect.x + target_box.rect.width / 2, target_box.rect.y + target_box.rect.height / 2));
        target_box.rect.x = pos_track_filed.x; //　添加roi偏移量
        target_box.rect.y = pos_track_filed.y;
    }
    //卡尔曼滤波预
    //发送数据层
    Point pos_predict_kalman = kalman_filter->predict(Point(target_box.rect.x + target_box.rect.width / 2, target_box.rect.y + target_box.rect.height / 2));
    cv::Rect2d kalman_predict_rect = target_box.rect;
    kalman_predict_rect.x = pos_predict_kalman.x;// - pos.width / 2.0;
    kalman_predict_rect.y = pos_predict_kalman.y;// - pos.height / 2.0;
    //前5帧放原始的track数据
    if(tracking_cnt<5){
        //cv::circle(srcImage,predict,5,cv::Scalar(0,255,255),2);
        //sendPosition(target_box.rect,srcImage);
    }
    //5帧后开始增加卡尔曼滤波做预测
    else{
        //sendPosition(kalman_predict_rect,srcImage);
    }
    //showArmorBox("tracker", srcImage, target_box.rect);
    return true;        //返回值为真
    //namedWindow("ROI",0);
    //resizeWindow("ROI",600,400);
    //imshow("ROI", roi);
    //waitKey(0);
}
