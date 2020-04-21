/*-----------------------------文件--------------------
*   文件名：FindLightBox.cpp
*   作者：  戴浪 代成浩
*   功能：  寻找装甲板上可能的灯条
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------

#include "ArmorFinder.h"

using namespace std;
using namespace cv;
void drawLightBlobs(cv::Mat &g_srcImage, const LightBlobs &blobs);
// 轮廓面积和其最小外接矩形面积之比
static double areaRatio(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect) {
    return cv::contourArea(contour) / rect.size.area();
}
// 旋转矩形的长宽比
static double lw_rate(const cv::RotatedRect &rect) {
    return rect.size.height > rect.size.width ?
           rect.size.height / rect.size.width :
           rect.size.width / rect.size.height;
}
// 判断轮廓是否为一个灯条
static bool isValidLightBlob(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect, BlobPartParam blob_parament) 
{
    /*
    Size2f cur_size = rect.size;
    float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//将矩形的长边设置为长
    float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//将矩形的短边设置为宽


    float length_width_ratio = length / width;//计算矩形长宽比
    if (length_width_ratio > blob_parament.BLOB_CONTOUR_HW_RATIO_MAX ||
        length_width_ratio < blob_parament.BLOB_CONTOUR_HW_RATIO_MIN) {
        //cout<<"length width ratio fail."<<endl;
//        cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
        return false;
        //长宽比不合适
        }
        */
    return (1.2 < lw_rate(rect) && lw_rate(rect) < 10) &&
           //           (rect.size.area() < 3000) &&
           ((rect.size.area() < 50 && areaRatio(contour, rect) > 0.4) ||
            (rect.size.area() >= 50 && areaRatio(contour, rect) > 0.6));
}
// 判断灯条颜色(此函数可以有性能优化).
static uint8_t get_blob_color(const cv::Mat &src, const cv::RotatedRect &blobPos) {
    auto region = blobPos.boundingRect();
    region.x -= fmax(3, region.width * 0.1);
    region.y -= fmax(3, region.height * 0.05);
    region.width += 2 * fmax(3, region.width * 0.1);
    region.height += 2 * fmax(3, region.height * 0.05);
    region &= cv::Rect(0, 0, src.cols, src.rows);
    cv::Mat roi = src(region);
    int red_cnt = 0, blue_cnt = 0;
    for (int row = 0; row < roi.rows; row++) {
        for (int col = 0; col < roi.cols; col++) {
            red_cnt += roi.at<cv::Vec3b>(row, col)[2];
            blue_cnt += roi.at<cv::Vec3b>(row, col)[0];
        }
    }
    if (red_cnt > blue_cnt) {
        return BLOB_RED;
    } else {
        return BLOB_BLUE;
    }
}
// 开闭运算
static void imagePreProcess(cv::Mat &src) {
    //开闭运算
    static cv::Mat kernel_erode = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernel_erode);

    static cv::Mat kernel_dilate = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernel_dilate);

    static cv::Mat kernel_dilate2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernel_dilate2);

    static cv::Mat kernel_erode2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernel_erode2);
}

/**
*@author：孙霖
*@name：blobParamInit()
*@return:bool
*@function:在给定图像上寻找所有可能的灯条
*@para:input_img输入图像:
*@返回：bool
**/ 
bool blobParamInit(BlobPartParam &blob_parament)
{
    blob_parament.RED_GRAY_THRESH = 180;//game
    blob_parament.BLUE_GRAY_THRESH = 100;//game
    blob_parament.SPLIT_GRAY_THRESH = 180;
    blob_parament.BLOB_CONTOUR_AREA_MAX = 500;
    blob_parament.BLOB_CONTOUR_AREA_MIN = 180;
    blob_parament.BLOB_CONTOUR_LENGTH_MIN = 10;
    blob_parament.BLOB_CONTOUR_LENGTH_MAX = 50;
    blob_parament.BLOB_CONTOUR_WIDTH_MIN = 0;
    blob_parament.BLOB_CONTOUR_WIDTH_MAX = 30;
    blob_parament.BLOB_CONTOUR_HW_RATIO_MAX = 22;
    blob_parament.BLOB_CONTOUR_HW_RATIO_MIN = 6; 
}
/**
*@author：孙霖
*@name：findLightBolbsSJTU()
*@return:bool
*@function:在给定图像上寻找所有可能的灯条
*@para:input_img输入图像:
*@返回：bool
**/ 
bool AutoAiming::findLightBolbsSJTU(cv::Mat &g_srcImage,cv::Mat &processImage,LightBlobs &light_blobs)
{
    //变量声明
    BlobPartParam blob_parament;        //灯条相关参数
    Mat color_channel;      //颜色通道
    Mat processImage_bin;     //亮源图和暗原图
    vector<Mat> channels;               //通道数
    split(processImage, channels);         //通道拆分  
    int light_threshold = 150;                //设定亮图片阈值

    //参数初始化
    blobParamInit(blob_parament);
    //cout<<"[1.1] 参数初始化"<<endl;
    // 对亮图片进行开闭运算
    if(mcu_data.enemy_color == ENEMY_BLUE){
        color_channel = channels[0];        //蓝色通道是1
    } else if(mcu_data.enemy_color == ENEMY_RED){
        color_channel = channels[2];        //红色通道是3
    }
    //二值化处理
    //亮度阈值    
    //cout<<"[1.2] 二值化"<<endl;
    threshold(color_channel, processImage_bin, light_threshold, 255, CV_THRESH_BINARY); // 二值化对应通道，得到较亮的图片
    //cout<<"[1.3] 图像预处理"<<endl;
    imagePreProcess(processImage_bin);
    //cout<<"[1.4] 显示预处理后的图像"<<endl;
    if(show_details_process && state == SEARCHING_STATE)
    {
        namedWindow("process_before_blob",0);
        resizeWindow("process_before_blob",600,400);
        imshow("process_before_blob", processImage_bin);
        waitKey(1);
    }
    // 使用两个不同的二值化阈值同时进行灯条提取，减少环境光照对二值化这个操作的影响。
    // 同时剔除重复的灯条，剔除冗余计算，即对两次找出来的灯条取交集。
    vector<vector<Point>> light_contours_light;    //创建存放轮廓的容器                       
    vector<Vec4i> hierarchy_light;              //            
    //cout<<"[1.5] 寻找边框"<<endl;    
    findContours(processImage_bin, light_contours_light, hierarchy_light, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); /*在经过二值化的较亮图片中进行轮廓提取*/ 
    //对light_contours_light中的轮廓用isValidLightBlob函数进行逐一比对，判断其是否为灯条，并若是将相关的信息存入
    for (int i = 0; i < light_contours_light.size(); i++) 
    {
        RotatedRect rect = minAreaRect(light_contours_light[i]);
        if (isValidLightBlob(light_contours_light[i], rect, blob_parament)) 
        {                         
            light_blobs.emplace_back(
                    rect, areaRatio(light_contours_light[i], rect), get_blob_color(processImage, rect)
            );
        }
    }
    //cout<<"total blobs number"<<light_blobs.size()<<endl;
    return light_blobs.size()>1;
}

/**
*@author： 代成浩 戴浪
*@name：showLightBlobs()
*@return:void
*@function:在图像上标记出已找到的灯条
*@para:input_img输入图像;light_blobs已找到的灯条
**/ 
//在src上将灯条框出
void AutoAiming::drawLightBlobs(Mat &g_srcImage, const LightBlobs &light_blobs)
{
    Mat img2show4blobs=g_srcImage.clone();
    for (const auto &blob:light_blobs) 
    {
        Scalar color(0,255,0);
        if (blob.blob_color == BLOB_RED)
            color = Scalar(0, 255, 0);
        else if (blob.blob_color == BLOB_BLUE)
            color = Scalar(0, 255, 0);
        Point2f vertices[4];
        blob.rect.points(vertices);
        for (int j = 0; j < 4; j++) 
        {
            line(img2show4blobs, vertices[j], vertices[(j + 1) % 4], color, 2);
        }
    }
    namedWindow("blobs",0);
    resizeWindow("blobs",600,400);
    imshow("blobs", img2show4blobs);
    waitKey(0);
}


