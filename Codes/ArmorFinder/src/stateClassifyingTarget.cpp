#include "stateClassifyingTarget.h"

using namespace cv;
using namespace std;


/**
 * @name:        AutoAiming::numberClassifyRoi
 * @par:         cv::Mat &g_srcImage,cv::Mat &g_processImage
 * @return:      ArmorBox
 * @function:    识别数字代码
 * */
ArmorBox StateClassifyingTarget::numberClassifyRoi(cv::Mat &g_srcImage,cv::Mat &g_processImage, armorBoxes armor_boxes)
{
    //-------------------------变量定义
    Mat roi_armor, roi_armor_ostu;  //数字区域
    int number_armor[armor_boxes.size()];   //每个装甲板的数字
    //cout<<"total　box number" << armor_boxes.size()<<endl;
    int target_id=0;
    int useful_number=0;
    vector<Mat> channels;                 //利用vector对象拆分
    ArmorBox target_box=armor_boxes[0];
    ArmorBox sort_curr = armor_boxes[0];
    //初始化目标装甲板
    target_box.rect = cv::Rect2d(0,0,0,0);
    target_box.id=-1;
    //------------------------操作
    //根据装甲板大小排序
    sort(armor_boxes.begin(), armor_boxes.end(), [&](const ArmorBox &a, const ArmorBox &b) {
            /*if (last_box.rect != cv::Rect2d()) {
                return getPointLength(a.getCenter() - last_box.getCenter()) <
                       getPointLength(b.getCenter() - last_box.getCenter());
            } */
            return (a.getarmorPixelS() - sort_curr.getarmorPixelS() <
                    b.getarmorPixelS() -  sort_curr.getarmorPixelS());
    });
    //优化ROI提取数字
    for(int i = 0; i< armor_boxes.size();i++)
    {
        cv::Rect box_rect_new=armor_boxes[i].rect;
        int move_width = box_rect_new.width/8;
        box_rect_new = box_rect_new + Point(move_width,-move_width);   //平移，也就是左上顶点的x坐标-100，y坐标+100
        box_rect_new = box_rect_new + Size(-move_width*3,move_width);    //缩放，左上顶点不变，宽度-100，高度+100

        g_srcImage(box_rect_new).copyTo(roi_armor);
        //大津法提取数字。
        split(roi_armor, channels);                     //调用通道拆分函数
        if(mcu_data.enemy_color == ENEMY_RED){
            roi_armor_ostu = channels[2];             //敌方是红色，红色是第三个通道
        }
        else{
            roi_armor_ostu = channels[0];             //敌方是蓝色，蓝色是第1个通道
        }
        //换成三色图
        cv::cvtColor(roi_armor_ostu, roi_armor_ostu, COLOR_GRAY2RGB);
        if(show_details_process)
        {
            imshow("ROI_NUMBER",roi_armor_ostu);
            waitKey(1);
        }
        //各个数字识别后储存
        number_armor[i] = getResultNumber(roi_armor_ostu);
        if(number_armor[i]!=0)
        {
            cout<<"Detected Armor Number of "<<i<<" : "<<number_armor[i]<<endl;
            target_id = i;
            useful_number++;
        }
    }
    //若没检测到数字则直接返回否
    if(useful_number==0)
    {
        cout<<"Detected No Number_Armor"<<" "<<endl;
        return target_box;
    }
    //目标是最后一个装甲板,即最近的装甲板 
    target_box = armor_boxes[target_id];
    //装甲板盒子没有大小
    if (target_box.rect == cv::Rect2d(0, 0, 0, 0)) 
    {   
        cout<<"Get No Boxes After Number"<<endl;
        return target_box;
    }
    //所有结束返回正确
    return target_box;
}

ArmorBox StateClassifyingTarget::run(cv::Mat &srcImage,cv::Mat &processImage,armorBoxes armor_boxes)
{
    if(show_state){
        cout<<"CLASSIFYING State start!"<<endl;
    }
    //识别ROI中的数字
    ArmorBox target_box=numberClassifyRoi(srcImage, processImage,armor_boxes);
    
    
    return target_box;
}