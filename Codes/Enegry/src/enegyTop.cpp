
#include"energy.h"
using namespace cv;
using namespace cv::ml;
using namespace std;
/*
 * 通过最小二乘法来拟合圆的信息
 * pts: 所有点坐标
 * center: 得到的圆心坐标
 * radius: 圆的半径
 */
static bool CircleInfo2(std::vector<cv::Point2f>& pts, cv::Point2f& center, float& radius)
{
    center = cv::Point2d(0, 0);
    radius = 0.0;
    if (pts.size() < 3) return false;;
    double sumX = 0.0;
    double sumY = 0.0;
    double sumX2 = 0.0;
    double sumY2 = 0.0;
    double sumX3 = 0.0;
    double sumY3 = 0.0;
    double sumXY = 0.0;
    double sumX1Y2 = 0.0;
    double sumX2Y1 = 0.0;
    const double N = (double)pts.size();
    for (int i = 0; i < pts.size(); ++i)
    {
        double x = pts.at(i).x;
        double y = pts.at(i).y;
        double x2 = x * x;
        double y2 = y * y;
        double x3 = x2 *x;
        double y3 = y2 *y;
        double xy = x * y;
        double x1y2 = x * y2;
        double x2y1 = x2 * y;
        sumX += x;
        sumY += y;
        sumX2 += x2;
        sumY2 += y2;
        sumX3 += x3;
        sumY3 += y3;
        sumXY += xy;
        sumX1Y2 += x1y2;
        sumX2Y1 += x2y1;
    }
    double C = N * sumX2 - sumX * sumX;
    double D = N * sumXY - sumX * sumY;
    double E = N * sumX3 + N * sumX1Y2 - (sumX2 + sumY2) * sumX;
    double G = N * sumY2 - sumY * sumY;
    double H = N * sumX2Y1 + N * sumY3 - (sumX2 + sumY2) * sumY;
    double denominator = C * G - D * D;
    if (std::abs(denominator) < DBL_EPSILON) return false;
    double a = (H * D - E * G) / (denominator);
    denominator = D * D - G * C;
    if (std::abs(denominator) < DBL_EPSILON) return false;
    double b = (H * C - E * D) / (denominator);
    double c = -(a * sumX + b * sumY + sumX2 + sumY2) / N;
    center.x = a / (-2);
    center.y = b / (-2);
    radius = std::sqrt(a * a + b * b - 4 * c) / 2;
    return true;
}


//获取点间距离
double getDistance(Point A,Point B)
{
    double dis;
    dis=pow((A.x-B.x),2)+pow((A.y-B.y),2);
    return sqrt(dis);
}




//#define USE_CAMERA
#define USE_TEMPLATE
#define SHOW_RESULT
#define SHOW_CIRCLE


int energyRun()
{
    /*int stateNum = 4;
    int measureNum = 2;
    KalmanFilter KF(stateNum, measureNum, 0);
    //Mat processNoise(stateNum, 1, CV_32F);
    Mat measurement = Mat::zeros(measureNum, 1, CV_32F);
    KF.transitionMatrix = (Mat_<float>(stateNum, stateNum) << 1, 0, 1, 0,//A 状态转移矩阵
        0, 1, 0, 1,
        0, 0, 1, 0,
        0, 0, 0, 1);
    //这里没有设置控制矩阵B，默认为零
    setIdentity(KF.measurementMatrix);//H=[1,0,0,0;0,1,0,0] 测量矩阵
    setIdentity(KF.processNoiseCov, Scalar::all(1e-5));//Q高斯白噪声，单位阵
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));//R高斯白噪声，单位阵
    setIdentity(KF.errorCovPost, Scalar::all(1));//P后验误差估计协方差矩阵，初始化为单位阵
    randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));//初始化状态为随机值*/

#ifdef USE_CAMERA
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M','J','P','G'));
#else
    VideoCapture cap;
    cap.open("material/blue.avi");
#endif

    Mat srcImage;
    cap >> srcImage;
    // 画拟合圆
    Mat drawcircle=Mat(srcImage.rows,srcImage.cols, CV_8UC3, Scalar(0, 0, 0));
#ifdef USE_TEMPLATE
    Mat templ[9];
    for(int i=1;i<=8;i++)
    {
        templ[i]=imread("../../material/enegy/template"+to_string(i)+".jpg",IMREAD_GRAYSCALE);
    }
#endif
    vector<Point2f> cirV;

    Point2f cc=Point2f(0,0);

    //程序主循环

    while(true)
    {
        cap >> srcImage;
        auto t1 = chrono::high_resolution_clock::now();

        //分割颜色通道
        vector<Mat> imgChannels;
        split(srcImage,imgChannels);
        //获得目标颜色图像的二值图
        Mat midImage2;
       int energy_color = ENERGY_BLUE;                //敌人为blue 
        if(energy_color == ENERGY_RED){
            midImage2=imgChannels.at(2)-imgChannels.at(0);
        } else if(energy_color == ENERGY_BLUE){
            midImage2=imgChannels.at(0)-imgChannels.at(2);}

        //二值化，背景为黑色，图案为白色
        //用于查找扇叶
        threshold(midImage2,midImage2,100,255,CV_THRESH_BINARY);
        int structElementSize=2;
        Mat element=getStructuringElement(MORPH_RECT,Size(2*structElementSize+1,2*structElementSize+1),Point(structElementSize,structElementSize));
        //膨胀
        dilate(midImage2,midImage2,element);
        //开运算，消除扇叶上可能存在的小洞
        structElementSize=3;
        element=getStructuringElement(MORPH_RECT,Size(2*structElementSize+1,2*structElementSize+1),Point(structElementSize,structElementSize));
        morphologyEx(midImage2,midImage2, MORPH_CLOSE, element);


        //查找轮廓
        vector<vector<Point>> contours2;
        vector<Vec4i> hierarchy2;
        findContours(midImage2,contours2,hierarchy2,CV_RETR_TREE,CHAIN_APPROX_SIMPLE);

        RotatedRect rect_tmp2;
        bool findTarget=0;

        //遍历轮廓
        if(hierarchy2.size())
            for(int i=0;i>=0;i=hierarchy2[i][0])
            {
                rect_tmp2=minAreaRect(contours2[i]);
                Point2f P[4];
                rect_tmp2.points(P);
                Point2f srcRect[4];
                Point2f dstRect[4];
                double width;
                double height;
                //矫正提取的叶片的宽高
                width=getDistance(P[0],P[1]);
                height=getDistance(P[1],P[2]);
                if(width>height)
                {
                    srcRect[0]=P[0];
                    srcRect[1]=P[1];
                    srcRect[2]=P[2];
                    srcRect[3]=P[3];
                }
                else
                {
                    swap(width,height);
                    srcRect[0]=P[1];
                    srcRect[1]=P[2];
                    srcRect[2]=P[3];
                    srcRect[3]=P[0];
                }
                //通过面积筛选
                double area=height*width;
                if(area>5000){
                    dstRect[0]=Point2f(0,0);
                    dstRect[1]=Point2f(width,0);
                    dstRect[2]=Point2f(width,height);
                    dstRect[3]=Point2f(0,height);
                    // 应用透视变换，矫正成规则矩形
                    Mat transform = getPerspectiveTransform(srcRect,dstRect);
                    Mat perspectMat;
                    warpPerspective(midImage2,perspectMat,transform,midImage2.size());
                    // 提取扇叶图片
                    Mat testim;
                    testim = perspectMat(Rect(0,0,width,height));
                    if(testim.empty())
                    {
                        cout<<"filed open"<<endl;
                        return -1;
                    }
#ifdef USE_TEMPLATE
                    cv::Point matchLoc;
                    double value;
                    Mat tmp1;
                    resize(testim,tmp1,Size(42,20));
#endif

#ifdef USE_TEMPLATE
                    vector<double> Vvalue1;
                    vector<double> Vvalue2;
                    for(int j=1;j<=6;j++)
                    {
                        value = TemplateMatch(tmp1, templ[j], matchLoc, CV_TM_CCOEFF_NORMED);
                        Vvalue1.push_back(value);
                    }
                    for(int j=7;j<=8;j++)
                    {
                        value = TemplateMatch(tmp1, templ[j], matchLoc, CV_TM_CCOEFF_NORMED);
                        Vvalue2.push_back(value);
                    }
                    int maxv1=0,maxv2=0;

                    for(int t1=0;t1<6;t1++)
                    {
                        if(Vvalue1[t1]>Vvalue1[maxv1])
                        {
                            maxv1=t1;
                        }
                    }
                    for(int t2=0;t2<2;t2++)
                    {
                        if(Vvalue2[t2]>Vvalue2[maxv2])
                        {
                            maxv2=t2;
                        }
                    }
#endif
                    //预测是否是要打击的扇叶
#ifdef USE_TEMPLATE
                    if(Vvalue1[maxv1]>Vvalue2[maxv2]&&Vvalue1[maxv1]>0.6)
#endif
                    {
                        findTarget=true;
                        //查找装甲板
                        if(hierarchy2[i][2]>=0)
                        {
                            RotatedRect rect_tmp=minAreaRect(contours2[hierarchy2[i][2]]);
                            Point2f Pnt[4];
                            rect_tmp.points(Pnt);
                            const float maxHWRatio=0.7153846;
                            const float maxArea=2000;
                            const float minArea=500;

                            float width=rect_tmp.size.width;
                            float height=rect_tmp.size.height;
                            if(height>width)
                                swap(height,width);
                            float area=width*height;

                            if(height/width>maxHWRatio||area>maxArea ||area<minArea){

                                 continue;
                            }
                            Point centerP=rect_tmp.center;
                            //打击点
                            circle(srcImage,centerP,1,Scalar(0,255,0),2);
                 
                        /*    Mat prediction = KF.predict();
                            Point predict_pt = Point((int)prediction.at<float>(0), (int)prediction.at<float>(1));
                           measurement.at<float>(0) = (float)centerP.x;
                           measurement.at<float>(1) = (float)centerP.y;
                           KF.correct(measurement);
                          circle(srcImage, predict_pt, 3, Scalar(34, 255, 255), -1);
                          centerP.x = (int)prediction.at<float>(0);
                          centerP.y = (int)prediction.at<float>(1);*/

#ifdef SHOW_CIRCLE
                            circle(drawcircle,centerP,1,Scalar(0,0,255),1);
                            //用于拟合圆，用30个点拟合圆
                            if(cirV.size()<30)
                            {
                                cirV.push_back(centerP);
                            }
                            else
                            {
                                float R;
                                //得到拟合的圆心
                                CircleInfo2(cirV,cc,R);
                                circle(drawcircle,cc,1,Scalar(255,0,0),2);
#endif
#if (defined DEBUG_LOG)&& (defined SHOW_CIRCLE)
                                cout<<endl<<"center "<<cc.x<<" , "<<cc.y<<endl;
#endif
#ifdef SHOW_CIRCLE
                                cirV.erase(cirV.begin());

                            }
                            if(cc.x!=0&&cc.y!=0){
                                Mat rot_mat=getRotationMatrix2D(cc,30,1);
#endif
#ifdef SHOW_CIRCLE
                                float sinA=rot_mat.at<double>(0,1);//sin(60);
                                float cosA=rot_mat.at<double>(0,0);//cos(60);
                                float xx=-(cc.x-centerP.x);
                                float yy=-(cc.y-centerP.y);
                                Point2f resPoint=Point2f(cc.x+cosA*xx-sinA*yy,cc.y+sinA*xx+cosA*yy);
                                circle(srcImage,resPoint,1,Scalar(0,255,0),10);
                               
                           }
#endif
                            for(int j=0;j<4;++j)
                            {
                                line(srcImage,Pnt[j],Pnt[(j+1)%4],Scalar(0,255,255),2);
                            }
                        }
                    }
                }


            }
#ifdef SHOW_RESULT
        imshow("Result",srcImage);
        if('q'==waitKey(1))break;
#endif
        //函数所花的时间
        auto t2 = chrono::high_resolution_clock::now();
        cout << "Total period: " << (static_cast<chrono::duration<double, std::milli>>(t2 - t1)).count() << " ms" << endl;
//        t1 = chrono::high_resolution_clock::now();
    }
    return 0;
}


