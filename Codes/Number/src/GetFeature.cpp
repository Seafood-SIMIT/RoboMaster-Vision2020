/*-----------------------------文件--------------------
*   文件名：GetFeature.cpp
*   作者：  seafood
*   功能：  提取数字特征
------------------------------------------------------*/
#include "GetFeature.h"
//命名空间
using namespace cv;
using namespace std;

float testFeature[25];
/**
 * @name        getFeature
 * @par         图像区域
 * @return      void
 * @function    提取图像特征
 * */
void getFeature(Mat roi_number_image,float features[25])
{
    int M,N;    //  用来储存m的宽和高
    int width,height,LineBytes;
    int bottom,top,zuo,you;
    bool flag;
    //转为灰度图
    cvtColor(roi_number_image,roi_number_image,COLOR_RGB2GRAY);
    //转化为二值图像
    threshold(roi_number_image,roi_number_image,100,255,THRESH_BINARY);//若要用该函数实现反色，可将THRESH_BINARY改为THRESH_BINARY_INV
    //图像反色
    bitwise_not(roi_number_image,roi_number_image);
    //寻找数字部分的上下左右坐标值：
    width = roi_number_image.cols;
    height = roi_number_image.rows;
    LineBytes = (width * 8 + 31)/32 * 4;
    int j=0;
    for(j = 0;j<height;j++)//从下往上寻找底部坐标
    {
        flag=false;
        for(int i = 0;i < width; i++)
        {
            if((int)roi_number_image.at<uchar>(j*LineBytes + i) == 0)
            {
                flag=true;
                break;
            }
        }
        if(flag)
        {
            break;
        }
    }
    bottom = j;
    for(j = height-1;j>0;j-- )//从上往下寻找顶部坐标
    {
        flag = false;
        for( int i = 0 ; i < width; i++ )
        {
            if( (int)roi_number_image.at<uchar>(j * LineBytes + i) == 0 )
            {
                flag = true;
                break;
            }
        }
        if(flag)
        {
            break;
        }
    }
    top = j;
    int i=0;
    for(i = 0; i < width; i++ )//从左往右寻找左端坐标
    {
        flag = false;
        for( int j = 0 ; j < height; j++ )
        {
            if( (int)roi_number_image.at<uchar>(j * LineBytes + i) == 0 )
            {
                flag = true;
                break;
            }
        }
        if(flag)
        {
            break;
        }
    }
    zuo = i;
    for(i = width-1; i>0; i-- )//从右往左寻找右端坐标
    {
        flag = false;
        for( int j = 0 ; j < height; j++ )
        {
            if( (int)roi_number_image.at<uchar>(j * LineBytes + i) == 0 )
            {
                flag = true;
                break;
            }
        }
        {
            break;
        }
    }
    you = i;
    M = you-zuo;//找到的区域的宽
    N = top-bottom;//找到的区域的高

    //计算图片的宽和高
    /*top = 0;
    zuo = 0;
    bottom = m.rows;
    you = m.cols;
**/
    int jef_x,jef_y;
    //cout<<"M = "<<M<<"; N = "<< N <<endl;
    //将找到的区域划分为5x5个子块，计算每子块内像素值的和
    for( i =bottom; i < top; i++)
    {
        for( j = zuo ; j < you; j++)
        {
            if((int)roi_number_image.at<uchar>(i * LineBytes + j) == 255)
            {
                jef_y = (float)(i-bottom)/N*5;//比原程序多*5
                jef_x = (float)(j-zuo)/M*5;
                features[(int)(jef_y)*5 + int(jef_x)]++;
            }
        }
    }
    //计算当前子块的平均值
    for(i = 0; i < 25;i++)
    {
        features[i] = features[i]/((M/5)*(N/5));
    }
}
/**
 * @name        ouDistance
 * @par         特征1、特征2
 * @return      float
 * @function    计算欧式距离
 * */
//
float ouDistance(float feature_1[25], float feature_2[25])
{   
    float distance = 0;
    //不要忘记初始化0,否则出错
    //根据欧氏距离计算公式，计算距离的平方
    for(int i = 0;i<25;i++)
    {
        distance+=(feature_1[i]-feature_2[i])*(feature_1[i]-feature_2[i]);
    }
    //对上述计算结果开根号
    return sqrt(distance);
}
/* *
 * @name        getResultNumber
 * @par         图像区域
 * @return      int
 * @function    计算数字识别
 * */
int getResultNumber(cv::Mat roi_armor_ostu)
{
    float min_ou;  //用来储存最小欧式距离
    int mini;   // 用来储存最小的欧式距离的数字号
    float ouDistanceValue[24]={0};
    //存储当前测试图像与已知的8*3=24个数字图像之间的欧式距离
    getFeature(roi_armor_ostu, testFeature);  
    
    int count_ou=0;
    for(int i = 1; i < 9; i++)//在原例子上，i=0;i<10改成i=1;i<9
    {
        for(int j=1;j<4;j++)
        {
            ouDistanceValue[count_ou] = ouDistance(testFeature, yangben_Feature[count_ou]);
            count_ou++;
            //cout<<"Distance of yangben and "<<i<<"is "<<ouDistanceValue[count_ou]<<endl;
        }
    }

    mini = 0;//在原例子上，0改成1
    min_ou = ouDistanceValue[0];//在原例子上，[0]改成[11]
    //给min赋个初始值，假设与数字1的距离最小

    count_ou=0;
    for(int i = 1; i < 9; i++)//在原例子上，i=0;i<10改成i=1;i<9
    {
        for(int j=1;j<4;j++)
        {
            if( min_ou > ouDistanceValue[count_ou])
            {
                //cout<<"Distance of yangben and "<<i<<" is "<<ouDistanceValue[count_ou]<<endl;
                min_ou = ouDistanceValue[count_ou];
                mini = i;
            }
            count_ou++;
        }

    }
    //剔除杂图
    /*if(min > 2)
    {
        return 0;
    }*/
    return mini;
}
/* *
 * @name        getYangbenFeatures
 * @par         图像区域
 * @return      void
 * @function    获得样本的特征值
 * */
void getYangbenFeatures(){
    int count_number=0, filename=0;
    for(int i = 1; i < 9; i++)//在原例子上，i=0;i<10改成i=1;i<9
    {
        for(int j=1;j<4;j++)
        {
            //文件名
            filename=i*10+j;
            string s = "material/picture/number/" + to_string(filename) + ".png";//to_string(k)：将数值k转化为字符串，返回对应的字符串
            
            Mat num_yangben = imread(s,1);                                  //读取文件
            getFeature(num_yangben, yangben_Feature[count_number]);         //获取样本特征
            count_number++;
        }
    }
}

