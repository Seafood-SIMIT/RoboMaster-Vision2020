#include "GetFeature.h"
using namespace cv;
using namespace std;
Mat testImage;
float testFeature[25];
int width,height,LineBytes;
int bottom,top,zuo,you;

float yangben_Feature[24][25]={0};
//提取图像特征
void getFeature(Mat m,float a[25])
{
    int M,N;    //  用来储存m的宽和高
    int i,j;
    bool flag;
    //转为灰度图
    cvtColor(m,m,COLOR_RGB2GRAY);
    
    //转化为二值图像
    threshold(m,m,100,255,THRESH_BINARY);//若要用该函数实现反色，可将THRESH_BINARY改为THRESH_BINARY_INV
    //cout<<"第一个点的值为："<<(int)m.at<uchar>(1)<<endl;
    //cout<<"read file "<<" type is "<<m.type()<<endl;
    //
    //图像反色
    bitwise_not(m,m);

    //imshow("number_hioghpo", m);
    //waitKey(1);
    //寻找数字部分的上下左右坐标值：
    width = m.cols;
    height = m.rows;
    LineBytes = (width * 8 + 31)/32 * 4;
    for(j = 0;j<height;j++)//从下往上寻找底部坐标
    {
        flag=false;
        for(i = 0;i < width; i++)
        {
            if((int)m.at<uchar>(j*LineBytes + i) == 0)
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
    for( j = height-1;j>0;j-- )//从上往下寻找顶部坐标
    {
        flag = false;
        for( i = 0 ; i < width; i++ )
        {
            if( (int)m.at<uchar>(j * LineBytes + i) == 0 )
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
    for( i = 0; i < width; i++ )//从左往右寻找左端坐标
    {
        flag = false;
        for( j = 0 ; j < height; j++ )
        {
            if( (int)m.at<uchar>(j * LineBytes + i) == 0 )
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
    for( i = width-1; i>0; i-- )//从右往左寻找右端坐标
    {
        flag = false;
        for( j = 0 ; j < height; j++ )
        {
            if( (int)m.at<uchar>(j * LineBytes + i) == 0 )
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
    for( i = 0 ; i < 25; i++)
    {
        a[i] = 0;
    }
    //cout<<"M = "<<M<<"; N = "<< N <<endl;
    //将找到的区域划分为5x5个子块，计算每子块内像素值的和
    for( i =bottom; i < top; i++)
    {
        for( j = zuo ; j < you; j++)
        {
            if((int)m.at<uchar>(i * LineBytes + j) == 255)
            {
                jef_y = (float)(i-bottom)/N*5;//比原程序多*5
                jef_x = (float)(j-zuo)/M*5;
                a[(int)(jef_y)*5 + int(jef_x)]++;
            }
        }
    }
    //计算当前子块的平均值
    for(i = 0; i < 25;i++)
    {
        a[i] = a[i]/((M/5)*(N/5));
    }
}

//计算欧式距离
float ouDistance(float a[25], float b[25])
{   
    int i;
    float distance = 0;
    //不要忘记初始化0,否则出错
    //根据欧氏距离计算公式，计算距离的平方
    for(i = 0;i<25;i++)
    {
        distance+=(a[i]-b[i])*(a[i]-b[i]);
    }
    //对上述计算结果开根号
    return sqrt(distance);
}
//计算数字识别
int getResultNumber(cv::Mat roi_armor_ostu)
{

    int i,j,k;
    float min;  //用来储存最小欧式距离
    int mini;   // 用来储存最小的欧式距离的数字号
    testImage = roi_armor_ostu;//*******此处读取待识别图像
    getFeature(testImage, testFeature);  
    //cout<<"testFeature "<<i<<"is "<<ouDistanceValue[count_ou]<<endl;
    /*for(i = 0; i < 25; i++)
    {
        cout<<testFeature[i]<<" to "<<yangben_Feature[3][i]<<endl;
    }*/
    //获取测试图像的特征值，并将其放到testFeature数组内
    
    //for(i = 0; i < 25; i++)
    //{
    //    cout<<testFeature[i]<<" to "<<srcFeature[11][i]<<endl;
    //}
    float ouDistanceValue[24]={0};
    //存储当前测试图像与已知的8*3=24个数字图像之间的欧式距离
    int count_ou=0;
    for(i = 1; i < 9; i++)//在原例子上，i=0;i<10改成i=1;i<9
    {
        for(j=1;j<4;j++)
        {
            ouDistanceValue[count_ou] = ouDistance(testFeature, yangben_Feature[count_ou]);
            count_ou++;
            //cout<<"Distance of yangben and "<<i<<"is "<<ouDistanceValue[count_ou]<<endl;
        }
    }

    mini = 0;//在原例子上，0改成1
    min = ouDistanceValue[0];//在原例子上，[0]改成[11]
    //给min赋个初始值，假设与数字1的距离最小

    count_ou=0;
    for(i = 1; i < 9; i++)//在原例子上，i=0;i<10改成i=1;i<9
    {
        for(j=1;j<4;j++)
        {
            if( min > ouDistanceValue[count_ou])
            {
                //cout<<"Distance of yangben and "<<i<<" is "<<ouDistanceValue[count_ou]<<endl;
                min = ouDistanceValue[count_ou];
                mini = i;
            }
            count_ou++;
        }

    }
    //剔除杂图
    if(min > 2.0)
    {
        return 0;
    }
    return mini;
}

int on_showImage_triggered(cv::Mat roi_armor_ostu)
{
    int mini = getResultNumber(roi_armor_ostu);
    //cout<<"识别该数字为："<<mini<<endl;
    return mini;
}

