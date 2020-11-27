/*-----------------------------文件--------------------
*   文件名：FindLightBox.cpp
*   作者：  seafood
*   功能：  寻找装甲板上可能的灯条
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------

#include "FindLightBlobs.h"

using namespace std;
using namespace cv;

//声明：
std::vector<std::vector<cv::Point> > light_contours;    //创建存放轮廓的容器               
std::vector<cv::Vec4i> hierarchy;              // 

bool blobParamInit(BlobPartParam &blob_parament);      //参数初始化
//定义：

/**
 * @name        blobParamInit()
 * @par         点矢量contour、rect矩阵、灯条参数
 * @return      bool
 * @function    在给定图像上寻找所有可能的灯条
 * */
bool blobParamInit(BlobPartParam &blob_parament)
{
    blob_parament.lw_ratio_min = 1.2;
    blob_parament.area_ration_min = 0.4;
    return 1;
}
/**
 * @name        LightBlob::LightBlob
 * @par         double x,double y
 * @return      double
 * @function    LightBlob构造函数
 * */
/*LightBlobs::LightBlobs(cv::RotatedRect &r, double ratio, uint8_t color) : rect(r), area_ratio(ratio), blob_color(color) 
{
    length = max(rect.size.height, rect.size.width);
};*/
/**
 * @name        max
 * @par         double x,double y
 * @return      double
 * @function    返回xy的最大值
 * */
static double max(double x,double y){
        return x>y?x:y;
    }
/**
 * @name        areaRatio
 * @par         Point矢量、矩形框
 * @return      double
 * @function    返回轮廓面积和其最小外接矩形面积之比
 * */
static double areaRatio(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect) {
    return cv::contourArea(contour) / rect.size.area();
}
/**
 * @name        lw_rate
 * @par         矩形
 * @return      double
 * @function    旋转矩形的长宽比
 * */
static double lw_rate(const cv::RotatedRect &rect) {
    return rect.size.height > rect.size.width ?
           rect.size.height / rect.size.width :
           rect.size.width / rect.size.height;
}
/**
 * @name        isValidLightBlob
 * @par         点矢量contour、rect矩阵、灯条参数
 * @return      bool
 * @function    判断轮廓是否为一个灯条
 * */
static bool isValidLightBlob(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect, BlobPartParam blob_parament) 
{
    return (blob_parament.lw_ratio_min < lw_rate(rect) && lw_rate(rect) < 10) &&
           //           (rect.size.area() < 3000) &&
           ((rect.size.area() < 50 && areaRatio(contour, rect) > 0.4) ||
            (rect.size.area() >= 50 && areaRatio(contour, rect) > 0.6));
}
/**
 * @name        imagePreProcess
 * @par         原图像
 * @return      void
 * @function    开闭运算
 * */
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
*@name：findLightBolbsSJTU()
*@return:bool
*@function:在给定图像上寻找所有可能的灯条
*@para:input_img输入图像:
**/ 
bool findLightBolbsSJTU(cv::Mat &g_srcImage,cv::Mat &processImage,lightBlobs &light_blobs)
{
    //变量声明
    BlobPartParam blob_parament;        //灯条相关参数
    Mat color_channel;      //颜色通道
    Mat processImage_bin;     //原图
    vector<Mat> channels;               //通道数
    int light_threshold;                //亮图片阈值

    split(processImage, channels);         //通道拆分  
    light_threshold = 130;                  //设定亮图片阈值
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
    if(show_details_process)
    {
        namedWindow("process_before_blob",0);
        resizeWindow("process_before_blob",600,400);
        imshow("process_before_blob", processImage_bin);
        waitKey(1);
    }
    //cout<<"[1.5] 寻找边框"<<endl;    
    findContours(processImage_bin, light_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); /*在经过二值化的较亮图片中进行轮廓提取*/ 
    //对light_contours_light中的轮廓用isValidLightBlob函数进行逐一比对，判断其是否为灯条，并若是将相关的信息存入
    for (int i = 0; i < light_contours.size(); i++) 
    {
        RotatedRect rect = minAreaRect(light_contours[i]);
        if (isValidLightBlob(light_contours[i], rect, blob_parament)) 
        {
            light_blobs.emplace_back(
                    rect, areaRatio(light_contours[i], rect), max(rect.size.height, rect.size.width)
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

void drawLightBlobs(Mat &g_srcImage, const lightBlobs &light_blobs)
{
    Mat img2show4blobs=g_srcImage.clone();
    for (const auto &blob:light_blobs) 
    {
        Scalar color(0,255,0);
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
    waitKey(1);
}
/**
*@author： seafood
*@name：LightBlob::run()
*@return:void
*@function:在图像上标记出已找到的灯条运行主函数
*@para:
**/
/*
void run(){
    if(!findLightBolbsSJTU())
    {
        return false;
    }
    //显示所有的灯条
    //cout<<"[2] Show Blobs"<<endl;
    //cout<<"Blobs after choose "<<light_blobs.size()<<endl;
    if (show_light_blobs && state==SEARCHING_STATE) 
    {
        drawLightBlobs(srcImage,light_blobs);
    }
}


*/