#include "ArmorFinder.h"

using namespace cv;
using namespace std;

bool AutoAiming::numberClassifyRoi(cv::Mat &g_srcImage,cv::Mat &g_processImage)
{
    Mat roi_armor, roi_armor_ostu;  //数字区域
    int number_armor[armor_boxes.size()];   //每个装甲板的数字
    target_box.rect = cv::Rect2d(0,0,0,0);
    target_box.id=-1;
    //cout<<"total　box number" << armor_boxes.size()<<endl;
    int target_id=0;
    int useful_number=0;
    for(int i = 0; i< armor_boxes.size();i++)
    {
        cv::Rect box_rect_new=armor_boxes[i].rect;
        int move_width = box_rect_new.width/8;
        //box_rect_new = armor_boxes[i].rect + Point(box_rect_new.width()/4,0);   //平移，也就是左上顶点的x坐标-100，y坐标+100
        //box_rect_new = armor_boxes[i].rect + Size(-box_rect_new.width()/4,0);    //缩放，左上顶点不变，宽度-100，高度+100

        box_rect_new = box_rect_new + Point(move_width,-move_width);   //平移，也就是左上顶点的x坐标-100，y坐标+100
        box_rect_new = box_rect_new + Size(-move_width*3,move_width);    //缩放，左上顶点不变，宽度-100，高度+100
	
        g_srcImage(box_rect_new).copyTo(roi_armor);
        //大津法提取数字。
        if (roi_armor.type() == CV_8UC3)
        {
            vector<Mat> channels;                 //利用vector对象拆分
            split(roi_armor, channels);                     //调用通道拆分函数
            roi_armor_ostu = channels[2];             //将红色提出来，红色是第三个通道
            //g_processImage_blue = channels[0];            //将蓝色提出来，红色是第一个通道
        } 
        cv::cvtColor(roi_armor_ostu, roi_armor_ostu, COLOR_GRAY2RGB);
        //imshow("ROI_NUMBER",roi_armor_ostu);
        //waitKey(1);
        number_armor[i] = on_showImage_triggered(roi_armor_ostu);
        if(number_armor[i]!=0)
        {
            cout<<"Detected Armor Number of "<<i<<" : "<<number_armor[i]<<endl;
            target_id = i;
            useful_number++;
        }
    }
    if(useful_number==0)
    {
        cout<<"Detected No Number_Armor"<<" "<<endl;
        return false;
    }
    target_box = armor_boxes[target_id];
    if (target_box.rect == cv::Rect2d(0, 0, 0, 0)) 
    {   
        cout<<"Get No Boxes After Number"<<endl;
        return false;

    }
    return true;
}